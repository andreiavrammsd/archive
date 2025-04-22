/*
 The C++ Programming Language, 4th Edition
 */

class Expr {
public:
    Expr() = default; // default constructor

    Expr(const Expr &) = default; // copy constructor

    virtual Expr *new_expr() = 0;

    virtual Expr *clone() = 0;
};

class Cond : public Expr {
public:
    Cond() = default;

    Cond(const Cond &) = default;

    Cond *new_expr() override {
        return new Cond();
    }

    Cond *clone() override {
        return new Cond(*this);
    }
};

void user(Cond *pc, Expr *pe) {
    Expr *pc2 = pc->new_expr();
    pc2->clone();

    Cond *pc3 = pc->clone();
    pc3->clone();

    Expr *pe2 = pe->new_expr();
    pe2->clone();

    // Cond *pe3 = pe->clone(); // error: Expr::clone() returns an Expr*
}

int main() {
    user(new Cond, new Cond);
}
