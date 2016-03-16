// http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
void
gen_random(char *s, const int len)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

string
randomString(size_t length)
{
    string str(length, 0);
    gen_random((char *) str.c_str(), length);
    return str;
}
