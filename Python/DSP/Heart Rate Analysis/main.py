from pylab import *
from scipy import signal
from scipy.io import wavfile
import matplotlib.pyplot as plt
import numpy as np

fsample, data = wavfile.read('LAB5_500HzFHR.wav')
m=60
lowcut =100/m
highcut=160/m
order=3

t=np.linspace(0,len(data)/fsample,len(data))

plt.plot(t, data)
plt.title("Heart-Rate signal in Bpm")
plt.xlabel("Time [s]")
plt.show()

#FFT

fft_data=fft(data)
f_unfiltered =fftfreq(len(data),1/fsample)
plt.plot(f_unfiltered,abs(fft_data))
plt.xlim(0)
plt.grid()
plt.title('FFT of Heart Rate')
plt.xlabel('Frequency(Hz)')
plt.show()
#Applying Filter

a,b= signal.butter(order,[lowcut,highcut],
                   btype='bandpass',fs=fsample)

filtered_data=signal.lfilter(a,b,data)
plt.plot(t,filtered_data)
plt.title("Heart-Rate signal in Bpm Filter Applied")
plt.xlabel("Time [s]")
plt.show()
#FFT Filter
fft_fil_data=fft(filtered_data)
f_filtered =fftfreq(len(filtered_data),1/fsample)
plt.plot(f_filtered,abs(fft_fil_data))
plt.xlim(0)
plt.grid()
plt.title('FFT of Heart Rate')
plt.xlabel('Frequency(Hz)')
plt.show()


###BPM

beats_unfiltered = 0
for i in range(1,len(data)):
    if (data[i] > 0) and (data[i-1] < 0):
        beats_unfiltered= beats_unfiltered + 1
print("Hearth Rate Signal(BPM):",int( beats_unfiltered* m * fsample/len(data)))

beats_filtered=0
for i in range(1,len(filtered_data)):
    if (filtered_data[i] > 0) and (filtered_data[i-1] < 0):
        beats_filtered= beats_filtered + 1
print("Filtered Hearth Rate Signal(BPM):",int( beats_filtered* m * fsample/len(filtered_data)))