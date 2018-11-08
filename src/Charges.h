//
// Created by krab1k on 8.11.18.
//

#pragma once

#include <string>
#include <vector>
#include <map>


class Charges {
    std::vector<std::string> names_{};
    std::map<std::string, std::vector<double>> charges_{};
public:
    std::vector<double> operator[](const std::string &name) const { return charges_.at(name); }

    void insert(const std::string &name, std::vector<double> charges);

    void save_to_file(const std::string &filename);
};
