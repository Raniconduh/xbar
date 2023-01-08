# xbar

Simple bar for X. Displays the root window name in the center of the bar.

E.g. the following shell script displays the date/time in the bar:

```sh
while :; do
	xsetroot -name "$(date)"
	sleep 1
done
```

## Configuration

Configuration can be done in the `config.h` file.


## Compilation & Installation

Requires Xlib.

Type `make` to compile, `make install` to install.
