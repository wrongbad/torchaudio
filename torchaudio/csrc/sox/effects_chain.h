#ifndef TORCHAUDIO_SOX_EFFECTS_CHAIN_H
#define TORCHAUDIO_SOX_EFFECTS_CHAIN_H

#include <sox.h>
#include <torchaudio/csrc/sox/utils.h>

namespace torchaudio::sox {

// Helper struct to safely close sox_effect_t* pointer returned by
// sox_create_effect

struct SoxEffect {
  explicit SoxEffect(sox_effect_t* se) noexcept;
  SoxEffect(const SoxEffect& other) = delete;
  SoxEffect(const SoxEffect&& other) = delete;
  auto operator=(const SoxEffect& other) -> SoxEffect& = delete;
  auto operator=(SoxEffect&& other) -> SoxEffect& = delete;
  ~SoxEffect();
  operator sox_effect_t*() const;
  auto operator->() noexcept -> sox_effect_t*;

 private:
  sox_effect_t* se_;
};

// Helper struct to safely close sox_effects_chain_t with handy methods
class SoxEffectsChain {
  const sox_encodinginfo_t in_enc_;
  const sox_encodinginfo_t out_enc_;

 protected:
  sox_signalinfo_t in_sig_;
  sox_signalinfo_t interm_sig_;
  sox_signalinfo_t out_sig_;
  sox_effects_chain_t* sec_;

 public:
  explicit SoxEffectsChain(
      sox_encodinginfo_t input_encoding,
      sox_encodinginfo_t output_encoding);
  SoxEffectsChain(const SoxEffectsChain& other) = delete;
  SoxEffectsChain(const SoxEffectsChain&& other) = delete;
  SoxEffectsChain& operator=(const SoxEffectsChain& other) = delete;
  SoxEffectsChain& operator=(SoxEffectsChain&& other) = delete;
  ~SoxEffectsChain();
  void run();
  void addInputTensor(
      torch::Tensor* waveform,
      int64_t sample_rate,
      bool channels_first);
  void addInputFile(sox_format_t* sf);
  void addOutputBuffer(std::vector<sox_sample_t>* output_buffer);
  void addOutputFile(sox_format_t* sf);
  void addEffect(const std::vector<std::string> effect);
  int64_t getOutputNumChannels();
  int64_t getOutputSampleRate();
};

} // namespace torchaudio::sox

#endif
