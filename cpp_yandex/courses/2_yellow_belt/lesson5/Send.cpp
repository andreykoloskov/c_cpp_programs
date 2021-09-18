#include <iostream>
#include <string>

using namespace std;

void SendSms(const string& number, const string& message) {
    cout << "Send '" << message << "' to number " << number << endl;
}

void SendEmail(const string& email, const string& message) {
    cout << "Send '" << message << "' to e-mail "  << email << endl;
}

/*
 Реализуйте здесь классы INotifier, SmsNotifier, EmailNotifier
 */

class INotifier {
public:
    virtual void Notify(const string& message) = 0;
};

class SmsNotifier : public  INotifier {
public:
    SmsNotifier(string number) : number_(number) {}

    void Notify(const string& message) override {
        SendSms(number_, message);
    }

private:
    string number_;
};

class EmailNotifier : public INotifier {
public:
    EmailNotifier(string email) : email_(email) {}

    void Notify(const string& message) override {
        SendEmail(email_, message);
    }

private:
    string email_;
};

void Notify(INotifier& notifier, const string& message) {
    notifier.Notify(message);
}
