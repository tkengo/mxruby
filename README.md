# MxRuby

Matrix or vector operations for ruby.

## Installation

Unavailable.

## Usage

```ruby
MX.new
```

## Rules

### function name

- `mx_***` is a function that is defined by ruby method.
- `mxx_***` is used from only C.
- `mxs_***` is also used from only C, but it is `static` function.

### copy function

- `mxx_copy_***` is copy function, which has interface that receive two arguments as src, dest.
