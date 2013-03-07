s.boot;
{CA1.ar(SampleRate.ir/20, MouseY.kr(16, 200), 18, 0, Impulse.kr(100))}.scope;
{CA1x.ar(10000, MouseY.kr(16, 200), 18, 0, Impulse.kr(100))}.scope;
s.quit;