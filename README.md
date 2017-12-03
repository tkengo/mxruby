# MxRuby

Matrix or vector operations for ruby.

## Installation

Unavailable.

## Development

Compile

```sh
$ bundle exec rake compile:mxruby
```

## Usage

```ruby
MX.new
```

## Rules

### function name

- `mx_***` is a function that is defined as a ruby method.
- `mxx_***` is only used from C.
- `mxs_***` is also only used from C, but it is a `static` function.

### copy function

- `mxx_copy_***` is copy function, which has interface that receive two arguments as src, dest.
