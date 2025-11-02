startd: 
	gcc main.c -o screamd && ./screamd
viewd:
	ps aux | grep screamd
stopd:
	kill $$(cat screamd.pid)
clean:
	rm screamd screamd.pid screamd.log

.PHONY= startd viewd stopd clean
