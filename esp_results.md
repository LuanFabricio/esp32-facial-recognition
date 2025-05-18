### MobileFaceNet:

- Runs = 5:
```sh
I (544641) [esp_cli]: Avg. inference time: 3875.9531
I (544641) [esp_cli]: Std. deviation: (+/-)0.491469740868
I (544651) [esp_cli]: Accuracy: 1.0000 (10/(5*2)|10)
```
- Runs = 10:
```sh
I (204051) [esp_cli]: Avg. inference time: 4650.7046
I (204051) [esp_cli]: Std. deviation: (+/-)816.658508300781
I (204061) [esp_cli]: Accuracy: 0.9500 (19/(10*2)|20)
```

### MobileFaceNet (Quant):

- Runs = 5:
```sh
I (77740) [esp_cli]: Avg. inference time: 3702.8262
I (77740) [esp_cli]: Std. deviation: (+/-)0.844139277935
I (77750) [esp_cli]: Accuracy: 0.8667 (13/(5*3)|15)
```
- Runs = 10:
```sh
I (149810) [esp_cli]: Avg. inference time: 3704.8096
I (149810) [esp_cli]: Std. deviation: (+/-)3905.212402343750
I (149820) [esp_cli]: Accuracy: 0.9000 (27/(10*3)|30)
```

### Rafael-student (Quant):

- Runs = 5
```sh
I (73686) [esp_cli]: Avg. inference time: 737.1805
I (73686) [esp_cli]: Std. deviation: (+/-)0.064226679504
I (73696) [esp_cli]: Accuracy: 0.8667 (13/(5*3)|15)

```
- Runs = 10
```sh
I (121776) [esp_cli]: Avg. inference time: 737.1820
I (121776) [esp_cli]: Std. deviation: (+/-)0.058555677533
I (121786) [esp_cli]: Accuracy: 0.9000 (27/(10*3)|30)
```
