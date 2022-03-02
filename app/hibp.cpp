#include "flat_file_db.hpp"
#include "hibp.hpp"
#include "os/bch.hpp"
#include "sha1.hpp"

int main(int argc, char* argv[]) {

    // build db
    // hibp::build_bin_db(std::cin, std::cout);
    // return 0;

    try {
        if (argc < 3)
            throw std::domain_error("USAGE: " + std::string(argv[0]) +
                                    " dbfile.bin plaintext_password");

        hibp::flat_file_db<hibp::pawned_pw> db(argv[1]);

        SHA1 sha1;
        sha1.update(argv[2]);
        hibp::pawned_pw needle = hibp::convert_to_binary(sha1.final());

        std::optional<hibp::pawned_pw> maybe_ppw;

        {
            os::bch::Timer t("search took");
            if (auto iter = std::lower_bound(db.begin(), db.end(), needle);
                iter != db.end() && *iter == needle) {
                maybe_ppw = *iter;
            } else {
                maybe_ppw = std::nullopt;
            }
        }

        std::cout << "needle = " << needle << "\n";
        if (maybe_ppw)
            std::cout << "found  = " << *maybe_ppw << "\n";
        else
            std::cout << "not found\n";

    } catch (const std::exception& e) {
        std::cerr << "something went wrong: " << e.what() << "\n";
    }

    return EXIT_SUCCESS;
}
