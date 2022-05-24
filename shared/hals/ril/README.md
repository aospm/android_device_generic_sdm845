# qrild

## Generating QMI C/headers

Install qmic from https://github.com/andersson/qmic

This MR is also needed if it's not merged: https://github.com/andersson/qmic/pull/3

```sh
cd src
cat qrild.qmi | qmic -a
```
