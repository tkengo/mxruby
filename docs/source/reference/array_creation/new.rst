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

  :rtype: Array
  :return: [*gamb*, *phib*, *psib*, *epsa*] -- see below.
  :raise: TypeError

  Returns
    * *gamb* (Float) -- Fukushima-Williams angle :math:`\bar{\gamma}`
    * *phib* (Float) -- Fukushima-Williams angle :math:`\bar{\phi}`
    * *psib* (Float) -- Fukushima-Williams angle :math:`\bar{\psi}`
    * *epsa* (Float) -- Fukushima-Williams angle :math:`\epsilon_A`

  Example
    .. code-block:: ruby

      Celes::pfw06(2450000.5, 5678.9)
      #=> [5.567146520539624e-06, 0.40906694795782306, 0.002764904877386836, 0.409066897588741] 
