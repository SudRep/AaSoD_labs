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
				uniform_int_distribution<int> char_dist(32, 126);

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
		for (size_t i = 0; i < _size; i++) {
			_data[i] = other._data[i];
		}
	}

	MyStack& operator=(const MyStack& other) {
		if (this != &other) {
			MyStack temp(other);
			swap(_data, temp._data);
			swap(_size, temp._size);
			swap(_capacity, temp._capacity);
		}
		return *this;
	}

	MyStack& operator+=(const T& value) {
		T copy_val = value;
		if (_size == _capacity) {
			size_t new_cap = _capacity * 2;
			T* new_data = new T[new_cap];
			for (size_t i = 0; i < _size; ++i) 
				new_data[i] = _data[i];
			delete[] _data;
			_data = new_data;
			_capacity = new_cap;
		}
		_data[_size++] = copy_val;
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
	
	~MyStack() {
		delete[] _data;
	}
};


double PostfixEvaluation(const string& src) {
	if (src.empty()) throw runtime_error("Invalid expression");

	MyStack<double> s;
	string buffer;

	for (char c : src) {
		if (isdigit(c) || c == '.')
			buffer += c;
		else  if (c == ' ') {
			if (!buffer.empty()) {
				s += stod(buffer);
				buffer.clear();
			}
		}
		else {
			if (!buffer.empty()) {
				s += stod(buffer);
				buffer.clear();
			}
			if (s.size() < 2) throw runtime_error("Invalid expression");

			double b = s.top();
			s.pop();
			double a = s.top();
			s.pop();

			double result = 0;
			switch (c) {
			case '+': result = a + b; break;
			case '-': result = a - b; break;
			case '*': result = a * b; break;
			case '/':
				if (b == 0.0) throw runtime_error("Division by zero");
				result = a / b;
				break;
			default: throw runtime_error("Unknown argument");
			}
			s += result;
		}
	}
	if (!buffer.empty())
		s += stod(buffer);

	if (s.size() != 1) throw runtime_error("Invalid expression");

	return s.top();
}

void TestPostfix() {
	cout << PostfixEvaluation("3 4 +") << endl;         
	cout << PostfixEvaluation("3 4 + 5 *") << endl;     
	cout << PostfixEvaluation("1 2 + 3 4 - *") << endl; 
	cout << PostfixEvaluation("10 3 -") << endl;        
	cout << PostfixEvaluation("7 2 /") << endl;
	try {
		PostfixEvaluation("10 0 /");
	} 
	catch (const exception& e) {
		cout << e.what();
	}
}

template<typename T>
void DemoAll(const string& type_name, const size_t count, const T& min, const T& max) {
	cout << "=== Demo for type: " << type_name << " ===" << endl;

	MyStack<T> s(count, min, max);
	cout << "Random " << type_name << " stack:" << endl << s << endl << endl;
	cout << "Stack size: " << endl << s.size() << endl << endl;
	s += s.top();
	cout << "Stack after += top(): " << endl << s << endl << endl;
	cout << "Stack size: " << endl << s.size() << endl << endl;

	cout << "top(): " << endl << s.top() << endl << endl;
	s.pop();
	s.pop();
	cout << "After 2 pop(): " << endl << s << endl << endl;

	MyStack<T> s_copy = s;
	cout << "s_copy == s:" << endl <<  (s_copy == s) << endl << endl;
	s_copy += min;
	cout << "s_copy after change != s " << endl << (s_copy != s) << endl << endl;

	MyStack<T> empty_s;
	cout << "Empty stack: " << endl << empty_s.empty() << endl << endl;
	try {
		empty_s.top();
	}
	catch (const exception& e) {
		cout << "Exception: " << e.what() << endl;
	}
	cout << "======== End Demo ========" << endl << endl;
}

int main() {
	DemoAll<int>("int", 5, 1, 100);
	DemoAll<double>("double", 5, 0.0, 1.0);
	DemoAll<char>("char", 7, 'a', 'z');
	DemoAll<string>("string", 3, "3", "8");
	TestPostfix();

	return 0;
}