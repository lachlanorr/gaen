//------------------------------------------------------------------------------
// main_composer.cpp - GUI editor program containing various tools
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#include "composer/stdafx.h"

#include "core/sockets.h"
#include "core/logging.h"

#include "composer/Messages.h"



// http://cpprocks.com/introduction-to-tr2-filesystem-library-in-vs2012/
// https://msdn.microsoft.com/en-us/library/hh874694%28v=vs.140%29.aspx      ---  C++ 14, the <filesystem> header VS2015
// https://msdn.microsoft.com/en-us/library/hh874694%28v=vs.120%29.aspx     --- <filesystem> header VS2013
// test
//#define STD_put_time_NOT_SUPPORTED
//#define NANA_FILESYSTEM_FORCE
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/categorize.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/filesystem/filesystem_ext.hpp>
namespace fs = std::experimental::filesystem;
using namespace nana::filesystem_ext;
//inline directory_only_iterator children(const fs::directory_entry& f) { return directory_only_iterator{ f.path() }; }
//inline fs::directory_iterator  l_items (const fs::directory_entry& f) { return fs::directory_iterator { f.path() }; }
//inline std::string             f_name  (const fs::directory_entry& f) { return f.path().filename().generic_u8string(); }
// the following types could be converted into types parametrs for a generic treelistpathview explorer
using d_node            = fs::directory_entry ;
using d_item            = fs::directory_entry ;
using ct_n_children     = directory_only_iterator;
using ct_l_items        = fs::directory_iterator ;
auto children = [](const d_node& f)->ct_n_children//&
                                {
                                        return ct_n_children{ f.path() };
                                };   // ct_n_children& f1(const d_node&);
auto l_items  = [](const d_node& f)->ct_l_items//&
                                {
                                        return ct_l_items   { f.path() };
                                };   // ct_l_items&  f2(const d_node&);
auto f_name   = [](const d_node& f)
                                {
                                        return  f.path().filename().generic_u8string();
                                };     // std::string  f3(const d_node&);
using f_node_children = decltype (children);
using f_list_items    = decltype (l_items);
using f_node_title    = decltype (f_name);
nana::listbox::oresolver& operator<<(nana::listbox::oresolver& ores, const d_node& item)
{
        ores << f_name(item);
    ores << pretty_file_date(item);//.path()
        if (fs::is_directory(item))
                ores << ("Directory") << "";
        else
        {
                if (item.path().has_extension())
                        ores << item.path().extension();
                else
                        ores << ("File");
        ores << pretty_file_size(item);
        }
        return ores;
}
nana::listbox::iresolver& operator>>(nana::listbox::iresolver& ires, d_node& m)
{
        /*std::string  type;
        ires >> m.name >> type >> type;
        m.directory = (type == "Directory");*/
        return ires;
}
//template <class d_node,
//            class d_item = d_node::value_type,
//                class ctn_node_children = std::vector<d_node>& F(d_node&) ,
//                class ctn_list_items    = std::vector<d_item>& F(d_node&) ,
//                class ft_node_title = std::string /* title_to_node */(const d_node::value_type& value)
//         >   // some data_node -like template
class explorer :public nana::form
{
        nana::place                place_ {*this};
        nana::treebox          tree_  {*this};
        nana::listbox          list_  {*this};
        nana::categorize<d_node> path_  {*this};
        nana::menubar          menu_  {*this};
        nana::panel<false>     view_  {*this},
                               status_{*this};
        nana::toolbar          tools_ {*this},
                               nav_   {*this};
        f_node_title           &node_to_title;
        f_node_children        &node_children;
        f_list_items           &list_items;
        std::string            div_    =
                R"(
                                vertical <weight=40 menu>
                         <weight=23 path>
                         <weight=23 tools>
                                         <<tree> |70%   < vertical <list> |50%  <vertical <weight=23 nav>
                                                                          <view>
                                                                >
                                         >
                         >N
                         <weight=23 status>
           )";
public:
        bool                               force_refresh{false};
        std::string            fake_item{"Sorry, this is a fake item!"};
        using t_node = nana::treebox::item_proxy;
        explorer ( /*d_node& root,*/
                       f_node_title     fnt,
                       f_node_children  ctnc,
                       f_list_items     ctni,
                       std::vector<std::pair<std::string, unsigned>> columns,
                       nana::rectangle r= nana::rectangle{ nana::point{50,10}, nana::size{900,600} },
                       std::string title={} )
        :form{r}, node_to_title{fnt}, node_children{ ctnc }, list_items{ ctni }
        {
                place_.div(div_.c_str());
                place_["menu"]  << menu_ ;
                place_["tools"] << tools_ ;
                place_["path"]  << path_ ;
                place_["tree"]  << tree_ ;
                place_["list"]  << list_ ;
                place_["nav"]   << nav_ ;
                place_["view"]  << view_ ;
                place_["status"]<< status_ ;
                place_.collocate();
                //t_node tree_root = add_root(root);
                tree_.events().selected( [this](const nana::arg_treebox &tb_msg)
                                                                 {
                                                                        if (!tb_msg.operated  ) return;
                                            //d_node d{ tree_.make_key_path(tb_msg.item, "/" /*nana::to_utf8(fs::path::preferred_separator)*/ ) 
                                                                        //              + "/" /*d_node::separator*/ };
                                                                        tree_.auto_draw(false);
                                                                        list_.auto_draw(false);
                                                                        //path_.auto_draw(false);
                                                                        refresh_tnode(tb_msg.item,      force_refresh);
                                                                        refresh_list(tb_msg.item);
                                                                        refresh_path(tb_msg.item);

                                                                        tree_.auto_draw(true);
                                                                        list_.auto_draw(true);
                                                                        //path_.auto_draw(true);
                                                                 });
                tree_.events().expanded([this](const nana::arg_treebox &tb_msg)
                                                                {
                                                                        tree_.auto_draw(false);
                                                                        refresh_tnode(tb_msg.item, force_refresh);
                                                                        tree_.auto_draw(true);
                                                                });
                //tree_root.select(true)  ;
                menu_.push_back("File");
                menu_.push_back("Help");
                for (auto & col : columns)
                        list_.append_header(col.first, col.second);
        }
        t_node add_root(d_node & root)
        {
                auto r= tree_.insert(node_to_title(root), node_to_title(root));
                r.value(root);
                signal_child(r);
                return r;
        }
        t_node add_root(std::string & root)
        {
                auto r = tree_.insert(root, root);
                r.value(d_node(root));
                signal_child(r);
                return r;
        }
        t_node add_root(std::string k, std::string t, d_node && root)   // revise ref types
        {
                auto r = tree_.insert(k, t);
                r.value(root);
                signal_child(r);
                return r;
        }
        void signal_child(t_node& node)
        {
                if (!force_refresh && !node.child().empty()) return;
                clear_children(node);
                auto& data = node.value<d_node>();
                const ct_n_children& d_c = node_children(data);
                const auto& child_1 = begin(d_c);
                if (child_1 != end(d_c))
                        tree_.insert(node, fake_item, "");
        }
        void clear_children(t_node& sel_node)
        {
                while (!sel_node.child().empty())
                        tree_.erase(sel_node.child());
        }
        void  refresh_list(t_node& sel_node)
        {
                list_.clear();
                const ct_l_items& items = list_items(sel_node.value<d_node>());
                for (auto &i : items)
                        list_.at(0).append(i, true);
        }
        void  refresh_path(t_node& sel_node) {};
        void  refresh_tnode(t_node& sel_node, bool force)
        {
                if (!force && !sel_node.child().empty() && sel_node.child().key()!=fake_item ) return;
                clear_children(sel_node);
                const ct_n_children& d_c = node_children(sel_node.value<d_node>());
                for (auto &n : d_c)
                {
                        auto name = node_to_title(n);
                        auto tn = tree_.insert(sel_node, name, name);
                        if (tn.empty()) continue;
                        tn.value(n);
                        signal_child(tn);
                }
        };
};
//using file_explorer_t = explorer< fs::directory_entry,   ///< d_node
//                                    fs::directory_entry,   ///< d_item
//                                                                decltype (children),   ///< ctn_node_children
//                                    decltype (l_items),        ///< ctn_list_items
//                                    decltype (f_name)      ///< ft_node_title
//                                >;
// The model / data known how to build a data_node from the path or from the value in the d_node atached to the t_node.
// The tree know how to build the path, and how to build a t_node from d_node
//using namespace nana;
using dir_it = directory_only_iterator;
template <class V,
class C_I,
class C_N
>
struct data_node
{
        using value_type = V;
        using cont_it_t = C_I;
        using cont_nd_t = C_I;
        value_type   value;
        C_I          items;   // any oder container of value_type with begin, end, != and ++()
        C_N          nodes;   // any oder container of data_node  with begin, end, != and ++()
        std::string  separator;
        data_node(std::string path, std::string  separator = "/")
                : value{ path },
                separator(separator)/*,
                                                        items{ path },
                                                        nodes{ path }*/ {}
};
// "globals functions"
std::string            key(/* value_type*/) { return{}; };
std::string            title(/* value_type*/) { return{}; };
struct dir_node
{
        using value_type = std::experimental::filesystem::directory_entry;
        using cont_it_t = std::experimental::filesystem::directory_iterator;
        using cont_nd_t = directory_only_iterator;
        value_type     value;
        cont_it_t      items;
        cont_nd_t      nodes;
        static const std::string separator;
        dir_node(std::experimental::filesystem::path path)
                : value{ path }, items{ path }, nodes { path }
        {}
                //separator{ "/"  },     /*  std::experimental::filesystem::path::preferred_separator  */
};
const std::string dir_node::separator{ "/" };
std::string            key(const dir_node& dn) { return dn.value.path().filename().generic_u8string(); };
std::string            title(const dir_node& dn) { return key(dn); };
std::string            key(const dir_it& d) { return d->path().filename().generic_u8string(); };
std::string            title(const dir_it& d) { return key(d); };







int CALLBACK WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    gaen::init_time();
    gaen::init_sockets();
    gaen::init_logging("127.0.0.1");


#if 0
    using namespace nana;
        // d_node d{ def_rootstr };
        explorer fb( f_name, 
                         children, 
                         l_items,
                                 { { ("Name"), 190 }, { ("Modified"), 145 }, { ("Type"), 80 }, { ("Size"), 70 }  });
        fb.add_root( def_root, def_rootname, d_node{ def_rootstr }).select(true);
    fb.show();
    nana::exec();
    //char a;
    //std::cin >> a;

#else

    using namespace gaen;

    Messages mw(nana::size(1280, 720));
    mw.show();

    //Start to event loop process, it blocks until the form is closed.
    nana::exec();
#endif
}
