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

### Compile

```bash
mkdir build
cd build && cmake .. && make -j
```

### Launch

```bash
./mimic3/mimic3
```

### Usage

```
mimic3 [-h] [--language LANG] [--noise-scale NOISE_SCALE] [--length-scale LENGTH_SCALE] [--noise-w NOISE_W] [--vocie VOICE] [--voices-dir VOICES_DIR] [--voices] [--output-dir OUTPUT_DIR] [--output-naming {text,time,id}]

Mimic 3 commad-line interface
positional arguments:

optional arguments:
  -h, --help: show this help message and exit
  --language LANG: language abbreviation pass to espeak-ng, default is en
      Check: https://github.com/espeak-ng/espeak-ng/blob/master/docs/languages.md
  --voice VOICE, -v VOICE: Name of voice (expected in <voices-dir>/<language>)
  --voice-dir VOICE: Directory with voices (format is <language>/<voice_name>)
  --vocies: List available voices
  --output-dir OUTPUT_DIR: Directory to write WAV file(s)
  --output-naming {text,time,id}: Naming scheme for output WAV files
  --id-delimiter ID_DELIMITER: Delimiter between id and text in lines (default: |)
  --length-scale LENGTH_SCALE: Length scale (1.0 is default speed, 0.5 is 2x faster)
  --noise-scale NOISE_SCALE: Noise scale [0-1], default is 0.667
  --noise-w NOISE_W: Variation in cadence [0-1], default is 0.8
```

### Prompt:

```
Hello World!
```

### Play sound

```bash
aplay ./0.wav
```
