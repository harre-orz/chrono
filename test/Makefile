TESTS := test_chrono test_chrono_sys test_chrono_mno test_chrono_cpu
CC := gcc
CFLAGS := -W -Wall -I../src/


all: $(TESTS)
	find $(TESTS) -exec ./{} \;

clean:
	rm -rf $(TESTS)
