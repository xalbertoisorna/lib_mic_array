#!/usr/bin/env python3
import subprocess
import numpy as np
from scipy.signal import chirp
import matplotlib.pyplot as plt

import soundfile as sf
import sounddevice as sd

import threading

def play_chirp():
    print("Playing chirp...")
    sample_rate = 44100
    t = np.linspace(0, 10, int(sample_rate * 10))
    chirp_signal = chirp(t, f0=10, f1=7000, t1=10, method='linear')
    chirp_signal *= 0.1 
    sd.play(chirp_signal, sample_rate)
    sd.wait()

threading.Thread(target=play_chirp, daemon=True).start()
subprocess.run(['xrun', '--xscope', 'bin/app_mic_array.xe'])
subprocess.run(['python', 'convert.py'])

# Plot both channels
audio, sr = sf.read('output.wav')
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6))
ax1.specgram(audio[:, 0], Fs=sr, cmap='viridis')
ax1.set_ylabel('Frequency (Hz)')
ax1.set_title('Channel 1')
ax2.specgram(audio[:, 1], Fs=sr, cmap='viridis')
ax2.set_ylabel('Frequency (Hz)')
ax2.set_xlabel('Time (s)')
ax2.set_title('Channel 2')
plt.tight_layout()
plt.savefig('analysis.svg')
plt.show()
