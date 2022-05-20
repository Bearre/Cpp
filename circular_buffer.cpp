template <class T>
class circular_buffer
{
	typedef circular_buffer_iterator<T> const_iterator;
	
	circular_buffer() = delete;
	public:
	explicit circular_buffer(size_t const size) : data_(size)
	{}
	
	bool clear() noexcept { head_ = -1; size_ = 0; }
	bool empty() const noexcept { return size_ == 0; }
	bool full() const noexcept { return size_ == data_.size(); }
	size_t capacity() const noexcept { return data_.size(); }
	size_t size() const noexcept { return size_; }
	
	void push(T const item)
	{
		head_ = next_pos();
		data_[head_] = item;
		if (size_ < data_.size()) size_++;
	}
  
  T pop()
	{
		if (empty()) throw std::runtime_error("empty buffer");
		auto pos = first_pos();
		size_--;
		return data_[pos];
	}
	
	const_iterator begin() const
	{
		return const_iterator(*this, first_pos(), empty());
	}
	
	const_iterator end() const
	{
		return const_iterator(*this, next_pos(), true);
	}
	
	private:
		std::vector<T> data_;
		size_t head_ = -1;
		size_t size_ = 0;
		
		size_t next_pos() const noexcept
		{ return size_ == 0 ? 0 : (head_ + 1) % data_.size(); }
		size_t first_pos() const noexcept
		{ return size_ == 0 ? 0 : (head_ + data_.size() - size_ + 1) % data_.size(); }
		
		friend class circular_buffer_iterator<t>;
};
