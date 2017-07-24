package main

import (
	"fmt"
	"os"
	"strings"
	"time"
)

func main() {
	var s, sep string
	start := time.Now()
	for i := 1; i < len(os.Args); i++ {
		s += sep + os.Args[i]
		sep = " "
	}
	fmt.Println(s, "in", time.Since(start))

	// Other version

	var s2, sep2 string
	start = time.Now()
	for _, arg := range os.Args[1:] {
		s2 = sep2 + arg
		sep = " "
	}
	fmt.Println(s2, "in", time.Since(start))
	// Yet another version
	start = time.Now()
	fmt.Println(strings.Join(os.Args[1:], " "), "in", time.Since(start))
}
