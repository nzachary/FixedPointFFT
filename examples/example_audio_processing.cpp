#include "fixedpointfft.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#define DR_WAV_IMPLEMENTATION
#include "deps/dr_wav/dr_wav.h"

using namespace FPFFT;

std::vector<int16_t> LoadWAV(const char* filePath, unsigned int& sampleRate)
{
  // Load the audio
  unsigned int audioChannels;
  drwav_uint64 audioLength;
  drwav_int16* audio = drwav_open_file_and_read_pcm_frames_s16(
      filePath, &audioChannels, &sampleRate, &audioLength, NULL);
  // Copy the audio to a vector
  std::vector<int16_t> out(audioLength);
  if (audioChannels == 0)
  {
    return std::vector<int16_t>();
  }
  else if (audioChannels == 1)
  {
    std::copy(audio, audio + audioLength, out.data());
  }
  else
  {
    // There are multiple channels; only use one of them
    const drwav_int16* inPtr = audio;
    int16_t* outPtr = out.data();
    for (size_t i = 0; i < audioLength; i++)
    {
      *outPtr++ = *inPtr;
      inPtr += audioChannels;
    }
  }
  // Free the original audio array
  drwav_free(audio, NULL);

  return std::move(out);
}

bool WriteWAV(
    const std::vector<int16_t>& audio, const char* filePath, const unsigned int sampleRate)
{
  drwav wav;
  drwav_data_format format{
      .container = drwav_container_riff,
      .format = DR_WAVE_FORMAT_PCM,
      .channels = 1,
      .sampleRate = sampleRate,
      .bitsPerSample = 16,
  };
  drwav_init_file_write(&wav, filePath, &format, NULL);

  drwav_uint64 written = drwav_write_pcm_frames(&wav, audio.size(), audio.data());

  return written == audio.size();
}


int main(int argc, char* argv[])
{
  // We can use any number of bits for the fraction here.
  // Any up/down scaling that happens when writing the input to the fixed point will be cancelled
  // out by down/up scaling when reading the output back to the original type.
  // The fraction bits may matter, however, if you do something other than just multiplying the FFT
  // output (such as adding or subtracting).
  using FPType = FixedPoint<int32_t, 24, int64_t>;
  using CType = Complex<FPType>;
  constexpr size_t FFTSize = 1024;
  constexpr size_t NumBins = FFTSize / 2 - 1;
  constexpr size_t Step = FFTSize / 4;

  if (argc < 4)
  {
    std::cout << "Usage: example_audio_processing [filter] [input wav] [output wav]" << std::endl;
    std::cout << "Filter types:" << std::endl;
    std::cout << "  L - Low pass filter" << std::endl;
    std::cout << "  H - High pass filter" << std::endl;
    std::cout << "  B - Band pass filter" << std::endl;
    std::cout << "  N - No filter" << std::endl;
    return -1;
  }

  // Create a plan to do the FFT and IFFT
  auto fftPlan = PlanCreator::PlanDFTC2C<FFTSize, FPType>();
  auto ifftPlan = PlanCreator::PlanIDFTC2C<FFTSize, FPType>();

  // Hann window function
  size_t startOffset = 0;
  std::array<FPType, FFTSize> hann; // Hann window function
  for (size_t n = 0; n < FFTSize; n++)
    hann[n] = std::pow(std::sin(M_PI * n / FFTSize), 2);

  // Filter to use on the audio
  std::array<FPType, NumBins> filter;
  for (size_t n = 0; n < NumBins; n++)
  {
    double s = (double)n / NumBins;
    switch (argv[1][0])
    {
      case 'L': // Low pass filter
      case 'l':
        filter[n] = 0.1 / (s + 0.1);
        break;
      case 'H': // High pass filter
      case 'h':
        filter[n] = s / (s + 1);
        break;
      case 'B': // Band pass filter
      case 'b':
        filter[n] = (0.2 < s && s < 0.4) ? 1 : 0;
        break;
      case 'N': // No filter
      case 'n':
        filter[n] = 1;
        break;
      default:
        std::cout << "Invalid filter; Expects one of: L, H, B, N" << std::endl;
        return -1;
        break;
    }
  }

  // Load some audio data
  unsigned int sampleRate;
  const std::vector<int16_t> audio = LoadWAV(argv[2], sampleRate);


  // The windows overlap so there needs to be a correction factor.
  const FPType correction = FPType(2.0 / 3);

  // Audio processing loop
  std::vector<int16_t> processedAudio(audio.size(), 0);
  while (audio.size() - startOffset > FFTSize)
  {
    // Copy the audio into the input
    CType sample;
    for (size_t n = 0; n < FFTSize; n++)
    {
      // We can write the audio directly, without any floating point conversions
      sample.Real().Storage() = audio[startOffset + n];
      // Apply windowing function
      sample.Real() *= hann[n];
      fftPlan.Input()(n) = sample;
    }

    // Execute the FFT
    fftPlan.Execute();

    // Apply a filter in the frequency domain
    Complex<FPType> c;
    for (size_t i = 0; i < NumBins; i++)
    {
      // Read the FFT output
      c = fftPlan.Output()(i);
      // Apply the filter
      c *= filter[i];
      // Pass that to the input of the IFFT
      ifftPlan.Input()(i) = c;
    }

    // Execute the IFFT
    ifftPlan.Execute();

    // Write the result of the IFFT to an audio output
    for (size_t i = 0; i < FFTSize; i++)
    {
      FPType v = ifftPlan.Output()(i).Real();
      // Apply the correction factor
      v *= correction;
      // We can also write the audio output directly, without any floating point conversions
      processedAudio[startOffset + FFTSize - i] += v.Storage();
    }

    startOffset += Step;
  }

  // Save the output
  if (WriteWAV(processedAudio, argv[3], sampleRate))
    return 0;

  // Write failed
  std::cout << "Failed to write audio to a file " << argv[3] << std::endl;
  return -1;
}
