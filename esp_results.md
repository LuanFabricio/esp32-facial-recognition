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
I (103550) [esp_cli]: Avg. inference time: 3703.1348
I (103550) [esp_cli]: Std. deviation: (+/-)0.608980715275
I (103570) [esp_cli]: Accuracy: 0.9333 (14/(5*3)|15)
```

- Runs = 10:
```sh
I (299820) [esp_cli]: Avg. inference time: 3703.1016
I (299830) [esp_cli]: Std. deviation: (+/-)0.606669604778
I (299840) [esp_cli]: Accuracy: 0.9333 (28/(10*3)|30)
```

### Rafael-student (Quant):

- Runs = 5
```sh
I (176166) [esp_cli]: Avg. inference time: 737.1843
I (176166) [esp_cli]: Std. deviation: (+/-)0.060791529715
I (176176) [esp_cli]: Accuracy: 0.9333 (14/(5*3)|15)

```
- Runs = 10
```sh
I (146706) [esp_cli]: Avg. inference time: 737.1812
I (146706) [esp_cli]: Std. deviation: (+/-)0.056116301566
I (146716) [esp_cli]: Accuracy: 0.9333 (28/(10*3)|30)
```
