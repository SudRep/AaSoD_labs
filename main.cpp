import std;
using namespace std;

template<typename T>
bool IsEqual(const T& a, const T& b) {
	if constexpr (is_floating_point_v<T>)
		return abs(a - b) < 1e-9;
	else
		return a == b;
	}



template<typename T>
class MyStack {
	T* _data;
	size_t _size;
	size_t _capacity;
public:
	MyStack() : _data(new T[10]), _size(0), _capacity(10) {}

	MyStack(size_t count, T min, T max ) : _data(new T[count * 2]), _size(count), _capacity(count*2)  {
		random_device rd;
		mt19937 gen(rd());

		for (size_t i = 0; i < _size; ++i) {
			if constexpr (is_same_v<T, int>) {
				if (min > max) throw invalid_argument("INT: min > max");
				uniform_int_distribution<int> dist(min, max);
				_data[i] = dist(gen);
			}
			else if constexpr (is_same_v<T, float> || is_same_v<T, double>) {
				if (min > max) throw invalid_argument("FLOAT/DOUBLE: min > max");
				uniform_real_distribution<T> dist(min, max);
				_data[i] = dist(gen);
			}
			else if constexpr (is_same_v<T, char>) {
				if (min > max) throw invalid_argument("CHAR: min > max");
				int code_min = static_cast<int>(min);
				int code_max = static_cast<int>(max);
				uniform_int_distribution<int> dist(code_min, code_max);
				_data[i] = static_cast<char>(dist(gen));
			}
			else if constexpr (is_same_v<T, string>) {
				int len_min = stoi(min);
				int len_max = stoi(max);
				if (len_min > len_max) throw invalid_argument("STRING: min > max");

				uniform_int_distribution<int> len_dist(len_min, len_max);
				std::uniform_int_distribution<int> char_dist(32, 126);

				int len = len_dist(gen);
				string s;
				s.reserve(len);
				for (int j = 0; j < len; ++j) {
					s += static_cast<char>(char_dist(gen));
				}
				_data[i] = s;
			}
		}
	}

	MyStack(const MyStack& other) :_size(other._size), _capacity(other._capacity), _data(new T[other._capacity]) {
		cout << "Copy constr" << endl;
		for (size_t i = 0; i < _size; i++) {
			_data[i] = other._data[i];
		}
	}

	MyStack& operator=(const MyStack& other) {
		cout << "Copy prisviv" << endl;
		if (this != &other) {
			MyStack temp(other);
			swap(_data, temp._data);
			swap(_size, temp._size);
			swap(_capacity, temp._capacity);
		}
		return *this;
	}

	MyStack& operator+=(const T& value) {
		if (_size == _capacity) {
			size_t new_cap = _capacity * 2;
			T* new_data = new T[new_cap];
			for (size_t i = 0; i < _size; ++i) 
				new_data[i] = _data[i];
			delete[] _data;
			_data = new_data;
			_capacity = new_cap;
		}
		_data[_size++] = value;
		return *this;
	}

	MyStack& operator--() {
		pop();
		return *this;
	}

	bool operator==(const MyStack& other) const {
		if (_size != other._size) return false;
		for (size_t i = 0; i < _size; ++i) {
			if (!IsEqual(_data[i], other._data[i])) return false;
		}
		return true;
	}

	bool operator !=(const MyStack& other) const {
		return !(*this == other);
	}

	friend ostream& operator<<(ostream& os, const MyStack& s) {
		os << "[";
		for (size_t i = s._size; i != 0; --i) {
			os << s._data[i - 1];
			if (i > 1) os << ", "; 
		}
		os << "]";
		return os;
	}

	void pop() {
		if (empty())
			throw out_of_range("Stack is empty");
		--_size;
	}

	const T& top() const {
		if (empty())
			throw out_of_range("Stack is empty");
		return _data[_size - 1];
	}

	bool empty() const { return _size == 0; }
	size_t size() const { return _size; }


	//временный метод для отладки
	const T* rawData() const { return _data; }
	

	~MyStack() {
		delete[] _data;
		cout << "dist" << endl;
	}
};



void TestPush()  {
	MyStack<string> s;
	cout << "Size before: " << s.size() << endl;   // 0
	cout << "Is empty: " << s.empty() << endl; // 1

	s += "abc";
	s += "xyz";
	s += "zxc";

	cout << "Size after: " << s.size() << endl; // 3
	cout << "Top: " << s.top() << endl; 
}

void TestPop() {
	MyStack<int> s;
	s += 1; 
	s += 2; 
	s += 3;

	cout << "Top: " << s.top() << endl; // 3
	--s;
	cout << "Top: " << s.top() << endl; // 2
	--s;
	cout << "Top: " << s.top() << endl; // 1
	--s;
	cout << "Is empty: " << s.empty() << endl; // 1
}

void TestExceptions() {
	MyStack<int> s;

	try {
		s.top();
	}
	catch (const out_of_range& e) {
		cout << e.what() << endl;
	}

	try {
		s.pop();
	}
	catch (const out_of_range& e) {
		cout <<  e.what() << endl;
	}
}

void TestAllTypes() {
	MyStack<char> sc(5, 'a', 'z');
	cout << "char stack, size = " << sc.size() << endl;
	for (size_t i = 0; i < sc.size(); ++i) {
		cout << sc.rawData()[i] << endl;
	}
	cout << endl;
	MyStack<float> sf(5, 0.0f, 1.0f);
	cout << "float stack, size = " << sf.size() << endl;
	for (size_t i = 0; i < sf.size(); ++i) {
		cout << sf.rawData()[i] << endl;
	}
	cout << endl;
	MyStack<double> sd(5, 0.0, 1.0);
	cout << "double stack, size = " << sd.size() << endl;
	for (size_t i = 0; i < sd.size(); ++i) {
		cout << sd.rawData()[i] << endl;
	}
	cout << endl;
	MyStack<string> ss(3, "3", "15");
	cout << "string stack, size = " << ss.size() << "\n";
	for (size_t i = 0; i < ss.size(); ++i) {
		cout << ss.rawData()[i] << endl;
	}
}

void EqualTest() {
	MyStack<int> a(5, 0, 100);
	MyStack<int> b(a);
	MyStack<float> c(5, 0.0f, 1.0f);
	MyStack<float> d(5, 0.0f, 1.0f);
	if (a == b)
		cout << "a == b" << endl;
	else
		cout << "a != b" << endl;
	if (d != c)
		cout << "d != c" << endl;
}

void TestOutput() {
	MyStack<int> s;
	s += 1; s += 2; s += 3;
	cout << s << endl;

	MyStack<char> sc;
	sc += 'a'; sc += 'b'; sc += 'c';
	cout << sc << endl;

	MyStack<std::string> ss;
	ss += "hello"; ss += "world";
	cout << ss << endl;
}


int main() {
	TestOutput();

	return 0;
}