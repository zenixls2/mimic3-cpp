# mimic3-cpp

Rewrite of mimic3 using c++. Not ready for production use.


### In Ubuntu:

```bash
sudo apt install -y libspdlog-dev libjsoncpp-dev libespeak-ng-dev libsndfile1-dev libutfcpp-dev
```

Download onnxruntime from github release page:

- https://github.com/microsoft/onnxruntime/releases (tested with `1.14.1`)
- unwrap it to `/usr/`
- in `/usr/lib`, run `ldconfig .`

may also refer: https://stackoverflow.com/questions/63420533/setting-up-onnx-runtime-on-ubuntu-20-04-c-api

### Pretrained Model Download

Check `https://github.com/MycroftAI/mimic3-voices/raw/master/voices/{lang}/{name}`. For example, the default voice should be `en_UK/apope_low`.
Download the files and put it to `~/.local/share/mycroft/mimic3/voices/{lang}/{name}`.

Or

```bash
pip install mycroft-mimic3-tts[all]
```

### Launch:

```bash
./mimic3/mimic3 ~/.local/share/mycroft/mimic3/voices/en_UK/apope_low/generator.onnx .
```

### Prompt:

```json
{"text": "Hello World", "output_path": "./out.wav", "mimic3": { "noise_scale": 0.067, "length_scale": 1.0, "noise_w": 0.2}}
```

### Play sound

```bash
aplay ./out.wav
```
