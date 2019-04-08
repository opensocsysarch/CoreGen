// {{{ MIT License

// Copyright 2017 Roland Kaminski

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

// }}}

#include <clingo/scripts.hh>
#include <gringo/logger.hh>

namespace Gringo {

bool Scripts::callable(String name) {
    if (context_ && context_->callable(name)) { return true; }
    for (auto &&script : scripts_) {
        if (script.second->callable(name)) {
            return true;
        }
    }
    return false;
}

void Scripts::main(Control &ctl) {
    for (auto &&script : scripts_) {
        if (script.second->callable("main")) {
            script.second->main(ctl);
            return;
        }
    }
}

SymVec Scripts::call(Location const &loc, String name, SymSpan args, Logger &log) {
    if (context_ && context_->callable(name)) { return context_->call(loc, name, args, log); }
    for (auto &&script : scripts_) {
        if (script.second->callable(name)) {
            return script.second->call(loc, name, args, log);
        }
    }
    GRINGO_REPORT(log, Warnings::OperationUndefined)
        << loc << ": info: operation undefined:\n"
        << "  function '" << name << "' not found\n"
        ;
    return {};
}

void Scripts::registerScript(clingo_ast_script_type type, UScript script) {
    if (script) { scripts_.emplace_back(type, std::move(script)); }
}

void Scripts::exec(ScriptType type, Location loc, String code) {
    bool notfound = true;
    for (auto &&script : scripts_) {
        if (script.first == static_cast<clingo_ast_script_type_t>(type)) {
            script.second->exec(type, loc, code);
            notfound = false;
        }
    }
    if (notfound) {
        std::ostringstream oss;
        oss << loc << ": error: ";
        switch (type) {
            case ScriptType::Python: { oss << "python"; break; }
            case ScriptType::Lua:    { oss << "lua"; break; }
        }
        oss << " support not available\n";
        throw GringoError(oss.str().c_str());
    }
}

char const *Scripts::version(clingo_ast_script_type type) {
    for (auto &&script : scripts_) {
        if (script.first == type) {
            return script.second->version();
        }
    }
    return nullptr;
}

Scripts::~Scripts() = default;

Scripts &g_scripts() {
    static Scripts scripts;
    return scripts;
}

} // namespace Gringo
