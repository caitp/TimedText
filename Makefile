topsrcdir="/home/caitp/git/timedtext"
topblddir="/home/caitp/git/timedtext/build"

all: timedtext check

timedtext:
	cd /home/caitp/git/timedtext && ./waf --out=/home/caitp/git/timedtext/build build

check:
	cd /home/caitp/git/timedtext && ./waf --out=/home/caitp/git/timedtext/build check

check-all:
	cd /home/caitp/git/timedtext && ./waf --out=/home/caitp/git/timedtext/build check --alltests

check-compact:
	cd /home/caitp/git/timedtext && ./waf --out=/home/caitp/git/timedtext/build check --compact

check-all-compact:
	cd /home/caitp/git/timedtext && ./waf --out=/home/caitp/git/timedtext/build check --alltests --compact

install:
	cd /home/caitp/git/timedtext && ./waf --out=/home/caitp/git/timedtext/build install

uninstall:
	cd /home/caitp/git/timedtext && ./waf --out=/home/caitp/git/timedtext/build uninstall

clean:
	cd /home/caitp/git/timedtext && ./waf --out=/home/caitp/git/timedtext/build clean
