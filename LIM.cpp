// Last modified on: 4/20/21
// Last modified by: Russell512
// Description: This is the cpp main file to implement a Lagrange Interpolation Method
// To run this file, just type in the command line(I guess): g++ LIM.cpp -o LIM, and then ./LIM

#include <iostream>
#include <vector>  

using namespace std;

// 找兩數最大公因數，利用輾轉相除法實踐
long long gcd(long long a, long long b) {
    // 可以研究兩數如何求得最大公因數
}

// 處理分數 (不過一開始的題目中提到 "The function that guarantees the answer will be a
// polynomial with all integer coefficients") 這個部分可能不需要，不過我認為要讓這個程式更完整
// 的話是必要的。啊分數處理這個部分就跟物件導向程式設計有關，可以稍微看一下。
// 知識點💡
// cpp 中有struct跟class，就是可以把很多的變數跟函數包裝在一起，這樣就可以方便的使用。
// 問題是為何要用？
// 在分數裡面，分子跟分母是神聖不可分割的，而且他們會有很多交互關係，像是自己的分母可能要跟別人通分啊
// 或是比如分子是3分母是6，我就要把這個分數變成1/2，啊電腦裡沒有真正儲存分數的變數
// 所以我們用class或是struct來包裝這些變數，這樣就可以方便的使用。
// 但其實在物件導向程式設計中，我們通常用class，但就是比較多功能，而struct就是比較簡單的，在這用struct就可
// 結論：struct讓我們可以存分子分母還有他們一大堆的功能。
struct Fraction {
    long long num, den; //初始化分子和分母
    // 初始化分數，這個東西叫做constructor建構子，當你建立一個物件的時候，這個函數就會被呼叫，會初始化這個物件裡的變數
    // 這個函數的名字跟class或struct名字一樣，沒有回傳值，這個函數就是用來初始化物件的
    Fraction(long long n = 0, long long d = 1) : num(n), den(d) {
        normalize();
    }
    // 標準化分數，就是把分數變成最簡分數，比如 2/4 就變成 1/2，或是讓分母不要是負數
    void normalize() {
        // 試試看吧！
    }
    // 知識點💡
    // 重載加法運算符
    // 為啥要重載？就是，我們本來可以用+號來做加法，但是+號只能對int float double等基本數據類型使用
    // 不能對我們這個自己建立的分數struct使用，所以我們要「重載」+號，讓+號可以用在我們設計的分數加法上
    Fraction operator+(const Fraction& other) const {
        // 這邊other就是其他分數，然後num, den就是我們自己，比如說1/2 (自己) + 3/4 (other)
        long long new_num = num * other.den + other.num * den;
        long long new_den = den * other.den;
        return Fraction(new_num, new_den);
    }
    // 乘法
    Fraction operator*(const Fraction& other) const {
        // 做做看
    }
};

// 多項式乘法，會回傳一個新的多項式
vector<Fraction> multiplyPolynomials(const vector<Fraction>& a, const vector<Fraction>& b) {
    // 一開始先新增一個"result"的vector，大小是多項式a的大小 + 多項式b的大小 - 1，並且初始化為0
    // 這個vector就是用來存放結果的
    vector<Fraction> result(a.size() + b.size() - 1, Fraction(0, 1));
    // 可以觀察到，多項式相乘時，係數會相乘，指數會相加，所以我們就是把這個規則實現在這個迴圈裡
    // 這步驟建議可以用紙筆寫一下，比較清楚
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            result[i + j] = result[i + j] + (a[i] * b[j]);
        }
    }
    return result;
}

// 多項式加法 <- 有點tricky，可以觀察加法的邏輯跟乘法不同的地方加以下手
vector<Fraction> addPolynomials(const vector<Fraction>& a, const vector<Fraction>& b) {
    // 請寫
}

int main() {
    int N;
    // 我們用vector來存放x和y，vector是一種酷陣列，他可以動態增加，不像一般的陣列大小是固定的，然後使用方法跟陣列一樣
    // 還有更多的功能，比如可以用push_back()來新增元素，或是用size()來取得大小
    // vector的宣告是 vector<data_type> array_name;
    vector<long long> x, y;
    cout << "Enter N and N data points (Xi Yi):\n";
    // Input
    cin >> N;
    for (int i = 0; i < N; i++) {
        long long xi, yi;
        cin >> xi >> yi;
        x.push_back(xi);
        y.push_back(yi);
    }

    int n = x.size();
    // 特例處理
    if (n == 0) {
        cerr << "No data points entered.\n";
        return 1;
    }

    // 初始化多項式，如果要將vector中的值初始化為value，可以用vector<data_type> array_name(size, value);
    vector<Fraction> polynomial(1, Fraction(0, 1));

    // Lagrange Interpolation Method
    for (int i = 0; i < n; i++) {
        // 做法是先初始化分子為1 (是一個vector)，分母為1 (是一個Fraction)
        vector<Fraction> numerator(1, Fraction(1, 1));
        long long denominator = 1;
        // 這個迴圈是用來建立多項式Li(x) <- 如果不知道這是啥請看pdf
        // for迴圈從x (是一個vector) 的0開始然後到最後一個，除了i以外都要 (這也是pdf講的)
        for (int j = 0; j < n; j++) {
            if (i != j) {
                // term是一個vector，裡面有兩個Fraction，第一個是-x[j]，第二個是1
                // -x[j]是 因為我們要建立的是(x-x1)(x-x2)...(x-xn)，所以要把xj變成-xj
                vector<Fraction> term;
                term.push_back(Fraction(-x[j], 1));
                term.push_back(Fraction(1, 1));
                // 以上 會得到 1*x + -x[j] = x - x_j 就是Li(x)的分子
                numerator = multiplyPolynomials(numerator, term);
                // 把之前得到的跟新的乘起來因為要連乘，分子部分就做完了
                // 下面這行請你寫分母，在Li(x)中，分母應該就是連乘 x_i - x_j
                // denominator = ?
            }
        }

        // Li(x) * yi
        for (int k = 0; k < numerator.size(); k++) {
            // 要怎麼乘上yi? 想想分數乘上一個數是「分子」還是「分母」在乘
            numerator[k] = Fraction(/*_________*/, /*__________*/);
        }

        // 連加
        // 這裡有一行請你寫，pdf中有寫到要「連加」結果，如何實踐？
    }

    // 印出多項式
    cout << "f(x) = ";
    bool first = true;
    // 這交給你自己看嘍 電腦的多項式表示法是：x^3+2x(-1/2) <-也可以把-從括號提出來
    for (int i = polynomial.size() - 1; i >= 0; i--) {
        if (polynomial[i].num != 0) {
            if (/*請判斷多項式何時需要'+'*/)
                //____________
            //下面這個if是印係數的，可研究
            if (i == 0 || abs(polynomial[i].num) != polynomial[i].den)
                cout << (polynomial[i].den == 1 ? to_string(polynomial[i].num) : "(" + to_string(polynomial[i].num) + "/" + to_string(polynomial[i].den) + ")");
            if (/*請判斷多項式何時需要印出'x'*/)
                //____________
            if (/*請判斷多項式何時需要印出'次方'*/)
                //____________
            first = false;
        }
    }
    cout << endl;
    return 0;
}