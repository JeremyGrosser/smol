

print('// Upstream')
for freq in range(902300000, 914900000, 200000):
    print('{.freq=%d, .min_dr=0, .max_dr=3, .cr=CR4_5},' % freq)

print('')

for freq in range(903000000, 914200000, 1600000):
    print('{.freq=%d, .min_dr=4, .max_dr=4, .cr=CR4_5},' % freq)

print('\n// Downstream')

for freq in range(923300000, 927500000, 600000):
    print('{.freq=%d, .min_dr=8, .max_dr=13, .cr=CR4_5},' % freq)
