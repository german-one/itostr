# itostr
### C function library to convert integral values of any type to null-terminated strings.

---

**The aim** of this little library is to provide functions that convert integral values to strings using a numerical base. This is similar to what `itoa` already does. But `itoa` is no C-standard function, so don't rely on its existence in all implementations. And there are some deficiencies that I try to overcome in my library functions:
- convert values of *any* integer type  
- check whether `NULL` was passed  
- check whether the buffer has a sufficient size  
- return a value that indicates whether the conversion failed  
- use the return value to indicate the length of the resulting string  

---

We need only two **public functions**. Actually, there are two pairs of functions, so a total of four functions. While one function converts to ASCII strings, its twin converts to wide strings.  
- function `signedtostr`: convert a signed value to an ASCII string  
- function `signedtowcs`: convert a signed value to a wide string  
- function `unsignedtostr`: convert an unsigned value to an ASCII string  
- function `unsignedtowcs`: convert an unsigned value to a wide string  

Find the reference of the required arguments in the [`itostr.h`](itostr.h) file.  
The behavior and usage of the functions is explained in the next paragraphs.

---

A few sentences about the **example code** in [`main.c`](main.c)  
I'm doing some funny business in these examples. Unsigned types get passed to the `signedto...` functions and signed types to the `unsignedto...` functions in order to explain the behavior of these functions.  

`USHRT_MAX` is passed to the `signedto...` function. It has value `65535` in most implementations.
This value does perfectly fit into the first parameter, still as a positive value. But since we want to treat it as a signed value of type `short`, the value shall be casted internally. Thus, the expected output is the converted value of `((short)USHRT_MAX)`, which is `-1` rather than `65535`.  

Similarly `SHRT_MIN` is passed to the `unsignedto...` function. It has value `-32768` in most implementations.
Leading bits get filled with ones if this value is casted for the first parameter. Provided `uintmax_t` has a size of 8, this value would be casted to `18446744073709518848`. But the expected output is the converted value of `((unsigned short)SHRT_MIN)`, which is `32768` in most implementations.  

The output of the examples is ...
```
length: 2       string:-1
length: 5       string:32768
```
... with a type size of 2 provided for `signed/unsigned short`.  
Depending on the real size of an `unsigned short` in your implementation, the output in the second line might be different.  

Well, this is not the intended use as I said in the beginning. Probably you rather want to convert a value using its actual signedness and size like that ...
```
char buffer[80] = { 0 };
short mynumber = -23;
size_t length = signedtostr(SIGNEDC(mynumber), sizeof(mynumber), buffer, sizeof(buffer), 10);
if (length != 0)
  puts(buffer);
```
Use the `unsignedto...` functions for unsigned types, respectively.  

---

If you are wondering what **SIGNEDC and UNSIGNEDC** are good for:  
These are function-like macros that cast your value into `intmax_t` or `uintmax_t`. Those are the types that the functions expect to receive for the first parameter. If you omit the type cast, your values will be implicitly casted to the right type anyway. Depending on your compiler settings you may get a warning though. The explicit type cast using the macros is just a way to tell your compiler: "Yes I know that the values are casted to a different type. I'm aware of the risks it carries but that's still what I want."  

---

Speaking of **risks** - these functions are way safer than common `itoa` implementations. But still I can't make them foolproof.  
- `value` parameter: You can't go wrong here unless you don't pass the value that you want to convert.  
- `valsize` parameter: The implementation already checks that the size is a power of two and that it doesn't exceed the maximum size for the `value` parameter. But if you pass a size that doesn't fit to your value, the resulting string will not meet your expectations.  
- `buffer` parameter: If you pass `NULL`, the return value will be zero and `buffer` will not be accessed. If you pass an array with at least one element, the implementation of the functions will always append a terminating null to the string to protect you from reading into invalid memory. Even if the functions fail, the resulting string is null-terminated but has zero length in this case. I can't protect you from passing any random addresses to this parameter though. It's your responsibility to pass either a valid pointer to an array of at least one character length, or a `NULL` pointer.  
- `bufsize` parameter: If this value is the size of the buffer in characters, the implementation checks whether or not the buffer is large enough for your converted string including the terminating null. If this parameter is greater than `0` the resulting string is always null-terminated. It's your responsibility to pass a value that is less than or equals the real length of your buffer.  
- `base` parameter: The implementation checks that the value is in range 2..36. Otherwise the functions will fail and return `0`.  
- return value: If the functions fail they return `0`. If you can't make sure that you didn't pass a `NULL` pointer to the `buffer` parameter or `0` to the `bufsize` parameter, don't access the memory that `buffer` points to.  
- Your implementation has to use *two's complement* to represent negative numbers. I'm pretty sure you would know if your implementation uses one's complement or sign-magnitude because that would be rather unusual. However, I included a preprocessor check in the header file that prevents compiling the code and informs the user why. The risk of getting wrong results is therefore close to zero.  

---

All files in this software package under **MIT license**.  
Copyright (c) 2019 Steffen Illhardt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

