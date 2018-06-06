compile:
	mkdir -p bin
	cd build && \
		cmake .. && \
		make -j$(shell nproc)

main: compile
	cd bin && \
		./Billing

clean:
	git clean -xdf bin

