#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "miproject"                         //change this
 
const char WIFI_SSID[] = "Shubham's Galaxy M51";               //change this
const char WIFI_PASSWORD[] = "1234@abcd";           //change this
const char AWS_IOT_ENDPOINT[] = "a1x4rget4tmruj-ats.iot.eu-west-1.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUE7sgdxMRoK54Y+smTiAa+5BhpLUwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMTAwODA4Mjc1
MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANoHv4THga6P0i4cMq4F
5nDv4U4NNwWztZgF7v7GnYEIwiTpVmCtZEzjwIpuI2tXJ4oEF8kqUE/Zu3miJ1/e
6r7BJiSEE5ySp3e9UP5FDgsk6f3jwR1zkSA2hJu15zDyKCrmeoT/I2QMI8llJww3
/88jzunkM7iWkcR7kJ/foieW6O/KHaoymYkxi+R5TsAgiAk3x50dWiuDkwm1V5Ud
W8wV4Pvsqf3CagnaKK62ZGFKlUQNaM8eFXtdnp6hZOi+iAHOv9aC9SHJztgBGoq6
r/gGw2LkHqn2MrWaw0IIR5sM7hjgG9vVlkZHAab2dR+Pr4n0eMZfypl8mORExO1a
TrUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUZKpOB1QviKZ9KIH6hczVnOXtUP4wHQYD
VR0OBBYEFLlxQ8gUXldESg6SgQZGmQl31Q0RMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCM2IBBTZcaq4rNVBl94HJbjgxq
VpO+6i8eVfe6GJcz4Irg3x5aLOCjoS/KpapvlJl+pYV8QvrqwbB544meebdOS6/X
nq75PJvelVorUWlVtOkhGkxlUXe2qQ02363d9UJP1aejch83jA34Tq3pHOSNpxVH
ULVfyWXGP9aUnW36iCoTsfi3brZhdVe9Jjgz/dwRzJPw/L/m288IaRFLeJK0M8m+
Xwdqq3qJ7KgEdltRzxqSd6TmXE4VuKSrmPt9e+Hzcei9yu4wtRyuO5gtMMpubVx9
m+40HSk0vBc8vXHmkN8tJzc47ba2flXyqNt8N40UBdUUQT3AjthoCqlgGhtQ
-----END CERTIFICATE-----

 
 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA2ge/hMeBro/SLhwyrgXmcO/hTg03BbO1mAXu/sadgQjCJOlW
YK1kTOPAim4ja1cnigQXySpQT9m7eaInX97qvsEmJIQTnJKnd71Q/kUOCyTp/ePB
HXORIDaEm7XnMPIoKuZ6hP8jZAwjyWUnDDf/zyPO6eQzuJaRxHuQn9+iJ5bo78od
qjKZiTGL5HlOwCCICTfHnR1aK4OTCbVXlR1bzBXg++yp/cJqCdoorrZkYUqVRA1o
zx4Ve12enqFk6L6IAc6/1oL1IcnO2AEairqv+AbDYuQeqfYytZrDQghHmwzuGOAb
29WWRkcBpvZ1H4+vifR4xl/KmXyY5ETE7VpOtQIDAQABAoIBAQCsnnvMNWR//1Ap
v5uEWhmDivg3opDQi+KXNQNuZXOoCHYlYTRJIJ6LnboAH4kdYAhm5iyq0Tx6q+kZ
OP+xkL0MP8HiIUuhqB681397gNLIkfZMU/i+R5Q8C83bth4Pi3mv1YBlMdckS9Q9
jD9pFiELL8+PMGSenV9VT3tGiHlKm2IbnzKfgIGfuAO+f/x89UJbZhqrrtGROq4o
DTV9vVO78sB8zpTL/EHEOdqmiMlyNpcWG87meOSWWAl4ccFagB/ozS0/O93uznpL
83fWYteWtVsiNHZApq5LYtcXKAr3bgm9yaYpApKloxzF4JL/6VjQMfTBPnxyhiJG
DvB5XYFhAoGBAP+BPLgU0BpErpUdE5h+TFR+/onAHgetGzEr0EqvqrwI88r0jSS4
WfSwwlPQ2sJhX9WFfzKvGWGm5GYn5odGRw3Dr46QsNFY+e//HkHUUgG353TuT72U
y637WEN8eoas4NJlLdNda56N3GRnWJVhnUszbVYjL7Ve0zZwCKKxTWS9AoGBANpz
6zY3MqnQyobE7oa3IFQKYbVU0THmn4aEL8UjrbvLcvp8Twnp2zCulTg8gXpEjknD
kFfAoRvHbOvomdR9tSIXQgUZMa/YwYmMsdNKf5pse3r/+KWE0r3UvmTxFDvLXofh
mn4e9gevY4hAFCmULPPaxkiCIBB82wZHpaj9an1ZAoGAd74fsL3AUa9CiouX9W2r
ZCYoMHKjIEHWbpL0CSyOvZUMfF9kDoN/VRfJ2vIUs+hiu3TXCe/Z21QV0nJ8rYUY
I7FEA3J4C4TBjeJoUb2puThw9vIA2P50/lK/KNGbsqYuryvP8DDmEOa3FS8B6vBW
gTnW0fmnkD6iie6zfVvOw5kCgYEAjcR6mdWakHAKbwmwjkabuU5/eHFRzaOQVXjF
k5eC8QOoW3x+wr08XUx/VF6X3GRPPvmmW/yL0X5LZpGt+yOVgEk9tBdUkn0FpKFk
6e1IAEx0mi8yCyC2rzE4WkPF0MQ7Vc2IOF8ZZYVh/5MuQomB3sNUuMuRyYFtlpa6
oH/NMwkCgYEA4BHMi0xSDBgtqp71zY1cSehaSKYLEgC3kIEKaq51lqwSwiOjQ856
PloIfOzpuP3VnGJvKwBYXTj3uS+pkon2hf2KiBpFXWhqhh3kIg4jjQtAfcgB2BvK
ft5xEhIQxQ5+VAaM2icyYO3BTNrXocjU2a8iGeH/Tk81EegBgtohHtA=
-----END RSA PRIVATE KEY-----

 
 
)KEY";