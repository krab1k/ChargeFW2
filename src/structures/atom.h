//
// Created by krab1k on 23/10/18.
//

#pragma once

#include <array>
#include <string>
#include <tuple>
#include <fmt/format.h>

#include "../element.h"


class Molecule;

class Atom {
    size_t index_{};
    const Element *element_{};
    std::array<double, 3> pos_{};
    const Molecule *molecule_{};
    size_t atom_type_{};
    int formal_charge_{};
    std::string atom_name_{};
    size_t residue_id_{};
    std::string residue_{};

    friend class Molecule;

    friend class MoleculeSet;

public:
    Atom(size_t index, const Element *element, double x, double y, double z, std::string atom_name, size_t residue_id,
         std::string residue);

    size_t index() const { return index_; }

    int formal_charge() const { return formal_charge_; }

    const Element &element() const { return *element_; }

    const Molecule *molecule() const { return molecule_; }

    const std::array<double, 3> &pos() const { return pos_; }

    size_t atom_type() const { return atom_type_; }

    size_t residue_id() const { return residue_id_; }

    std::string residue() const { return residue_; }

    std::string name() const { return atom_name_; }

    bool inline operator==(const Atom &other) const {
        return this->index_ == other.index_ and this->molecule_ == other.molecule_;
    }
};


namespace fmt {
    template<>
    struct formatter<Atom> {
        template<typename ParseContext>
        constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

        template<typename FormatContext>
        auto format(const Atom &a, FormatContext &ctx) {
            return format_to(ctx.begin(), "Atom {} Idx: {}", a.name(), a.index());
        }
    };
}
