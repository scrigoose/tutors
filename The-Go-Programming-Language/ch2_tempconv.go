package main

import "fmt"

type Celsius float64
type Fahrenheit float64
type Kelvin float64

const (
	AbsoluteZeroC Celsius = -273.15
	FreezingC     Celsius = 0
	BoilingC      Celsius = 100
)

func CToF(c Celsius) Fahrenheit {
	return Fahrenheit(c*9/5 + 32)
}

func CToK(c Celsius) Kelvin {
	return Kelvin(c + 273.15)
}

func FToC(f Fahrenheit) Celsius {
	return Celsius((f - 32) * 5 / 9)
}

func FToK(f Fahrenheit) Kelvin {
	return Kelvin(CToK(FToC(f)))
}

func KToC(k Kelvin) Celsius {
	return Celsius(k - 273.15)
}

func KToF(k Kelvin) Fahrenheit {
	return Fahrenheit(CToF(KToC(k)))
}

func (c Celsius) String() string {
	return fmt.Sprintf("%g°C", c)
}

func (f Fahrenheit) String() string {
	return fmt.Sprintf("%g°F", f)
}

func (k Kelvin) String() string {
	return fmt.Sprintf("%gK", k)
}

func main() {
	fmt.Println("FreezingC:", FreezingC.String())
	fmt.Println("BoilingC:", BoilingC.String())
	fmt.Println("AbsoluteZeroC:", AbsoluteZeroC.String())
	fmt.Println("FrezingF:", CToF(FreezingC))
	fmt.Println("BoilingF:", CToF(BoilingC))
	fmt.Println("AbsoluteZeroF:", CToF(AbsoluteZeroC))
	fmt.Println("FrezingK:", CToK(FreezingC))
	fmt.Println("BoilingK:", CToK(BoilingC))
	fmt.Println("AbsoluteZeroK:", CToK(AbsoluteZeroC))
	fmt.Println("100°F:", FToC(100), FToK(100))
	fmt.Println("100K:", KToC(100), KToF(100))
}
