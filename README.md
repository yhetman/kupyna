# kupyna

---

### Implementation of Kupyna hash function.

The Kupyna hash function was approved as the new Ukrainian standard **DSTU 7564:2014** in 2015. Main requirements for it were both high security level and good performance of software implementation on general-purpose 64-bit CPUs. The new hash function uses Davies-Meyer compression function based on Even-Mansour cipher construction. Kupyna is built on the transformations of the Kalyna block cipher (**Ukrainian standard DSTU 7624:2014**).


---

### Compiling

---

```
git clone https://github.com/yhetman/kupyna
cd kupyna
make
```

To reocompile the code use:

```
make re
```

### Usage

---

```
Usage:  ./kupyna -i ./input_file -o ./output_file
```

### Options and flags

---

```

Flags:
	-i  -- specify ./path/to/input/file
	-o  -- specify ./path/to/output/file
	-h  -- use this flag to see usage
```
