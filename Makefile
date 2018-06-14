main: deps.update debug
	cd bin && \
		./Billing

debug:
	mkdir -p bin
	cd build && \
		cmake -DCMAKE_BUILD_TYPE=Debug .. && \
		make -j$(shell nproc)

release:
	mkdir -p bin
	cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release .. && \
		make -j$(shell nproc)

clean:
	git clean -xdf bin

deps.update:
	git submodule update --init --recursive

