main: compile
	cd bin && \
		./Billing

compile:
	mkdir -p bin
	cd build && \
		cmake .. && \
		make -j$(shell nproc)

clean:
	git clean -xdf bin

