DTYPES = [ :int8, :int16, :int32, :int64, :float32, :float64 ]
DTYPES_INT = [ :int8, :int16, :int32, :int64 ]
DTYPES_FLOAT = [ :float32, :float64 ]
DTYPES_PRIO = Hash[DTYPES.map.with_index{|v,i| [ v, i ]}]
