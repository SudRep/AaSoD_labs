import std;
using namespace std;



template<typename T>
class MyStack {
	T* _data;
	size_t _size;
	size_t _capacity;
public:
	MyStack() : _data(new T[10]), _size(0), _capacity(10) {}

	MyStack(size_t count, T min, T max ) : _data(new T[count]), _size(count), _capacity(count*2)  {
		if (min > max) {
			throw invalid_argument("min cannot be greather than max");
		}
		random_device rd;
		mt19937 gen(rd());

		for (size_t i = 0; i < _size; ++i) {
			if constexpr (is_same_v<T, int>) {
				uniform_int_distribution<int> dist(min, max);
				_data[i] = dist(gen);
			}
			else if constexpr (is_same_v<T, float> || is_same_v<T, double>) {
				uniform_real_distribution<T> dist(min, max);
				_data[i] = dist(gen);
			}
			else if constexpr (is_same_v<T, char>) {
				int code_min = static_cast<int>(min);
				int code_max = static_cast<int>(max);
				uniform_int_distribution<int> dist(code_min, code_max);
				_data[i] = static_cast<char>(dist(gen));
			}
			else if constexpr (is_same_v<T, string>) {
				int len_min = stoi(min);
				int len_max = stoi(max);

				uniform_int_distribution<int> len_dist(len_min, len_max);
				std::uniform_int_distribution<int> char_dist(32, 126);

				for (size_t i = 0; i < _size; ++i) {
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
	}

	 //временный метод для отладки
	const T* rawData() const { return _data; }
	size_t getsize() const { return _size; }

	~MyStack() {
		delete[] _data;
	}



};


int main() {

	MyStack<string> s(5, "3", "8");
	for (size_t i = 0; i < s.getsize(); ++i) {
		cout << s.rawData()[i] << endl;
	}

	return 0;
}