cd output
cmake ..
make -j 4
if [ $? -eq 0 ]; then
	cd codex_test
	./codex_test
fi
