cd output
cmake ..
make -j 4
if [ $? -eq 0 ]; then
	cd codex.base.tests
	./codex.base.tests
fi
