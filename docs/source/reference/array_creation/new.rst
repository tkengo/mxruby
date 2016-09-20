=======
MX.new
=======

.. rb:module:: MX

.. rb:function:: new(array, opt = nil)

  Create mxarray from Ruby array.

  :param array: Ruby array.
  :type  date1: Array
  :param opt: Options hash. The default value is nil.
  :type  date2: Hash

  :return: mxarray

  :raise: TypeError

  Options
    * *dtype* -- data type. See also :doc:`/users_guide/data_type`

Examples
--------

  It passes 1-dimention array.

  .. code-block:: ruby

    MX.new([1, 2, 3])
    #=> [1.0, 2.0, 3.0]

  .. code-block:: ruby

    MX.new([1, 2, 3], dtype: :int64)
    #=> [1, 2, 3]

  It passes n-dimention array.

  .. code-block:: ruby

    MX.new([[1, 2, 3], [4, 5, 6]])
    #=> [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]]

  .. code-block:: ruby

    MX.new([[[1, 2], [3, 4]], [[5, 6], [7, 8]]])
    #=> [[[1.0, 2.0], [3.0, 4.0]], [[5.0, 6.0], [7.0, 8.0]]]
