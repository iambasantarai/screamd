startd: clean
	gcc main.c -o screamd && ./screamd
viewd:
	ps aux | grep screamd
stopd:
	kill $$(cat screamd.pid)
clean:
	rm -f screamd screamd.pid screamd.log
logs:
	tail -f screamd.log

.PHONY= startd viewd stopd clean logs
