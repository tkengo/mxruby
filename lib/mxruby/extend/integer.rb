class Integer
  alias_method :old_add, :+
  alias_method :old_mul, :*

  def +(other)
    if !other.nil? && other.is_a?(MX)
      return other + self
    end

    self.old_add(other)
  end

  def *(other)
    if !other.nil? && other.is_a?(MX)
      return other * self
    end

    self.old_mul(other)
  end
end
