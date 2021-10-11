%  @(#)60       1.1  src/sys/ibrsdbq.pro, os2-brs.src.sys, os2-brs.2.0, 931013 93/06/02 12:00:17
%  db_define(Name/Arity,                Index,  TableSize(default=4000)).

:- db_define(pd_directory/4,            [1,4]).
:- db_define(pd_file/6,                 [1,6]).
:- db_define(pd_class/3,                [1]).
:- db_define(pd_function/11,            [1]).
:- db_define(pd_variable/6,             [1]).
:- db_define(pd_block/1,                [1]).
:- db_define(pd_enumeraton_tag/2,       [1]).
:- db_define(pd_enumerator/3,           [1]).
:- db_define(pd_macro/3,                [1]).
:- db_define(pd_typedef/3,              [1]).
:- db_define(pd_label/2,                [1]).
:- db_define(pd_template/4,             [1]).
:- db_define(pd_try/1,                  [1]).
:- db_define(pd_catch/5,                [1]).
:- db_define(pd_throw/4,                [1]).
:- db_define(pd_defined/4,              [1,2,3]).
:- db_define(pd_declared/4,             [1,2,3]).
:- db_define(pd_used/4,                 [1,2,3],                50000).
:- db_define(pd_used_implicit/4,        [1,2,3]).
:- db_define(pd_used_lvalue/4,          [1,2,3]).
:- db_define(pd_member/4,               [1,2]).
:- db_define(pd_friend/5,               [1,2,5]).
:- db_define(pd_inherit/6,              [1,2,6]).
:- db_define(pd_include/3,              [1,2]).
:- db_define(pd_instantiated/2,         [1,2]).
:- db_define(pd_initializer/3,          [1,3]).
:- db_define(pd_exception_specifier/5,  [1]).
:- db_define(pd_source2xcoff/3,         [1,2]).
:- db_define(pd_typeof_variable/2,      [1]).
:- db_define(pd_typeof_function/4,      [1]).
:- db_define(pd_typeof_typedef/2,       [1]).

%%%%%% command line interface

id(ID, Name) :-
        pd_directory(ID,Name,_, _).
id(ID, Name) :-
        pd_file(ID,Name,_,_,_, _).
id(ID, Name) :-
        pd_class(ID,Name,_).
id(ID, Name) :-
        pd_function(ID,Name,_,_,_, _,_,_,_,_, _).
id(ID, Name) :-
        pd_variable(ID,Name,_,_,_, _).
id(ID, Name) :-
        pd_enumerator(ID,Name,_).
id(ID, Name) :-
        pd_enumeration_tag(ID,Name).
id(ID, Name) :-
        pd_macro(ID,Name,_).
id(ID, Name) :-
        pd_typedef(ID,Name,_).
id(ID, Name) :-
        pd_label(ID,Name).
id(ID, Name) :-
        pd_template(ID,Name,_,_).

%%%

defined(ID, FileID, Line, Column) :-
        pd_defined(ID,BlockID,Line,Column),
        block_in_file(BlockID, FileID).

declared(ID, FileID, Line, Column) :-
        pd_declared(ID,BlockID,Line,Column),
        block_in_file(BlockID, FileID).

used(ID, FileID, Line, Column) :-
        pd_used(ID,BlockID,Line,Column),
        block_in_file(BlockID, FileID).
used(ID, FileID, Line, Column) :-
        pd_used_implicit(ID,BlockID,Line,Column),
        block_in_file(BlockID, FileID).
used(ID, FileID, Line, Column) :-
        pd_used_lvalue(ID,BlockID,Line,Column),
        block_in_file(BlockID, FileID).

%%%

directory(ID) :-
        pd_directory(ID,_,_,_).

file(ID) :-
        pd_file(ID,_,_,_,_, _).

variable(ID) :-
        pd_variable(ID,_,_,_,_, _).

function(ID) :-
        pd_function(ID,_,_,_,_, _,_,_,_,_, _).

class(ID) :-
        pd_class(ID,_,class).
class(ID) :-
        pd_template(ID,_,class,_).

struct(ID) :-
        pd_class(ID,_,struct).
struct(ID) :-
        pd_template(ID,_,struct,_).

union(ID) :-
        pd_class(ID,_,union).
union(ID) :-
        pd_class(ID,_,static_union).
union(ID) :-
        pd_template(ID,_,union,_).

enumeration_tag(ID) :-
        pd_enumeration_tag(ID,_).

enumerator(ID) :-
        pd_enumerator(ID,_,_).

typedef(ID) :-
        pd_typedef(ID,_,_).

macro(ID) :-
        pd_macro(ID,_,_).

try(ID) :-
        pd_try(ID).

catch(ID) :-
        pd_catch(ID,_,_,_,_).

throw(ID) :-
        pd_throw(ID,_,_,_).

template(ID) :-
        pd_template(ID,_,_,_).

label(ID) :-
        pd_label(ID,_).

block(ID) :-
        pd_block(ID).

%%%

auto(ID) :-
        pd_variable(ID,_,_,auto,_, _).

register(ID) :-
        pd_variable(ID,_,_,register,_, _).

static(ID) :-
        pd_variable(ID,_,_,static,_,_).
static(ID) :-
        pd_function(ID,_,_,static,_, _,_,_,_,_, _).
static(ID) :-
        pd_class(ID,_,static_union).

extern(ID) :-
        pd_variable(ID,_,_,extern,_,_).
extern(ID) :-
        pd_function(ID,_,_,extern,_, _,_,_,_,_, _).

extern_c(ID) :-
        pd_variable(ID,_,_,extern_c,_, _)
        ;
        pd_function(ID,_,_,extern_c,_, _,_,_,_,_, _).

constructor(ID) :-
        pd_function(ID,_,_,constructor,_, _,_,_,_,_, _).

destructor(ID) :-
        pd_function(ID,_,_,destructor,_, _,_,_,_,_, _).

public(ID) :-
        pd_member(_,ID,public,_).

protected(ID) :-
        pd_member(_,ID,protected,_).

private(ID) :-
        pd_member(_,ID,private,_).

virtual(ID) :-
        pd_function(ID,_,_,_,_, _,_,_,virtual,_, _).

pure_virtual(ID) :-
        pd_function(ID,_,_,_,_, _,_,_,pure_virtual,_, _).

non_virtual(ID) :-
        pd_function(ID,_,_,_,_, _,_,_,no_specifier,_, _).

const(ID) :-
        pd_variable(ID,_,_,_,true, _)
        ;
        pd_function(ID,_,_,_,true, _,_,_,_,_, _).

volatile(ID) :-
        pd_variable(ID,_,_,_,_, true)
        ;
        pd_function(ID,_,_,_,_, _,_,_,_,_, true).

inline(ID) :-
        pd_function(ID,_,_,_,_, true,_,_,_,_, _).

operator(ID) :-
        pd_function(ID,_,_,_,_, _,_,true,_,_, _).

overload(ID) :-
        pd_function(ID,_,_,_,_, _,true,_,_,_, _).

override(ID) :-
        pd_function(ID,_,_,_,_, _,_,_,_,override, _).

compiler_generated(ID) :-
        pd_function(ID,_,_,_,_, _,_,_,_,compiler_generated, _).

%%%

include(ID1,ID2) :-
        pd_include(ID1,ID2,_).

inherit(ID1,ID2,Access,Virtual) :-
        pd_inherit(ID1,ID2,Access,Virtual,_, _).

friend(ID,ClassID) :-
        pd_friend(ID,ClassID,_,_, _).

instantiated(ID, TempID) :-
        pd_instantiated(ID,TempID).

%%%

local(ID, FunctionID) :-
        nonvar(ID), !,
        \+ pd_block(ID),
        pd_defined(ID,BlockID,_,_),
        block_in_function(BlockID, FunctionID).
local(ID, FunctionID) :-
        var(ID), !,
        block_in_function(BlockID,FunctionID),
        pd_defined(ID,BlockID,_,_),
        \+ pd_block(ID).

member(ID,ClassID,Access) :-
        pd_member(ClassID,ID,Access,_).

inherited_member(ID,ClassID,BaseClassID,Access) :-
        inherit_closure(ClassID,BaseClassID),
        pd_member(BaseClassID,ID,Access,_).

inherit_closure(ClassID,BaseClassID) :-
        inherit_closure(ClassID,BaseClassID,false)
        ;
        group([ClassID,BaseClassID],inherit_closure(ClassID,BaseClassID,true)).

inherit_closure(ClassID1,ClassID2,Virtual) :-
        pd_inherit(ClassID1,ClassID2,_,Virtual,_, _).
inherit_closure(ClassID1,ClassID2,Virtual) :-
        nonvar(ClassID1),!,
        pd_inherit(ClassID1,ClassIDT,_,CurrentVirtual,_, _),
        inherit_closure(ClassIDT,ClassID2,RestVirtual),
        or_op(CurrentVirtual, RestVirtual, Virtual).
inherit_closure(ClassID1,ClassID2,Virtual) :-
        var(ClassID1),!,
        pd_inherit(ClassIDT,ClassID2,_,CurrentVirtual,_, _),
        inherit_closure(ClassID1,ClassIDT,RestVirtual),
        or_op(CurrentVirtual, RestVirtual, Virtual).

% refers(ID1, ID2)
%
%    ID1  \ ID2  variable function class macro enum_tag enumerator typedef
% function        o        o(*1)    o      o      o       o          o
% class           x        x        o      o      o       x          o
% try             o        o        o      o      o       o          o
% catch           o        o        o      o      o       o          o
%
%     *1 : same as call/2

refers(ID1, ID2) :-
        nonvar(ID1),!,
        (
                function(ID1)
                ;
                class(ID1); struct(ID1); union(ID)
                ;
                try(ID1); catch(ID1)
        ),
        block_in_block(BlockID, ID1),
        (
                pd_used(ID2,BlockID,_,_)
                ;
                pd_used_lvalue(ID2,BlockID,_,_)
                ;
                pd_used_implicit(ID2,BlockID,_,_)
        ).
refers(ID1, ID2) :-
        var(ID1),!,
        (
                pd_used(ID2,BlockID,_,_)
                ;
                pd_used_lvalue(ID2,BlockID,_,_)
                ;
                pd_used_implicit(ID2,BlockID,_,_)
        ),
        block_in_block(BlockID,ID1),
        (
                function(ID1)
                ;
                class(ID1); struct(ID1); union(ID)
                ;
                try(ID1); catch(ID1)
        ).

calls(ID1,ID2) :-
        nonvar(ID1),!,
        group(ID2, (
                block_in_function(BlockID, ID1),
                (
                        pd_used(ID2,BlockID,_,_)
                        ;
                        pd_used_lvalue(ID2,BlockID,_,_)
                        ;
                        pd_used_implicit(ID2,BlockID,_,_)
                )
        )),
        function(ID2).
calls(ID1,ID2) :-
        var(ID1), !,
        function(ID2),
        group(ID1, (
                (
                        pd_used(ID2,BlockID,_,_)
                        ;
                        pd_used_lvalue(ID2,BlockID,_,_)
                        ;
                        pd_used_implicit(ID2,BlockID,_,_)
                ),
                block_in_function(BlockID, ID1)
        )).

% closure(Predicate, ID1, ID2)
%
%                 ID1 ::= <variable in Predicate>
%                 ID2 ::= <variable in Predicate>
%
%                  x  closure(p(a,a,B), a, B)
%                  o  closure(p(A,a,B), A, B), A = a

closure(P,ID1,ID2) :-
        general_closure(P,ID1,ID2).

%%%%%% Database Control

%%% for pb

pr_update(Path,RC) :-
                retract('$update'(_))
                ;
                brs_file(_,Path),
                unload_old_brs(Path),
                assert('$update'(Path)),
                fail
                ;
                retract('$update'(Path)),
                sconsult(Path,RC).

pr_load_match(PathExp,Path,RC) :-
        ignore_signal(2,(       % SIGINT
                find_file(PathExp, Path),
                unload_old_brs(Path),
                fail
                ;
                find_file(PathExp, Path),
                sconsult(Path,RC)
        )).

pr_unloadable(Path) :-
        brs_file(_, Path).

pr_unload_match(PathExp) :-
        ignore_signal(2,(       % SIGINT
                brs_file(_, Path),
                wildcard_match(Path, PathExp),
                unload_brs(Path)
        )).

pr_inconsistent_file(SrcName) :-
        pd_source2xcoff(SrcID,BrsID,_),
        pd_file(SrcID,_,SrcName,_,_,_),
        pd_file(BrsID,_,BrsName,_,_,_),
        filemodtime(SrcName,SrcTime),
        filemodtime(BrsName,BrsTime),
        SrcTime > BrsTime.

%%% for XCOFF translator %%%
%%% called by sconsult/1, sconsult/2 and unsconsult/1 %%%

pr_remove_location(PID) :-
        pd_defined(CID, PID, _, _),
        pr_remove_location(CID).
pr_remove_location(BlockID) :-
        retract(pd_defined(_,BlockID,_,_)).
pr_remove_location(BlockID) :-
        retract(pd_declared(_,BlockID,_,_)).
pr_remove_location(BlockID) :-
        retract(pd_used(_,BlockID,_,_)).
pr_remove_location(BlockID) :-
        retract(pd_used_implicit(_,BlockID,_,_)).
pr_remove_location(BlockID) :-
        retract(pd_used_lvalue(_,BlockID,_,_)).

%%% local

brs_file(ID, Path) :-
        group(ID, (
                pd_source2xcoff(_, ID, _),
                pd_file(ID, _, Path, _, _, _)
        )).

unload_old_brs(Path) :-
        brs_file(ID,Path),
        pd_file(ID,_,_,OldTime,_,_),
        filemodtime(Path,CurrTime),
        OldTime =\= CurrTime,
        (unload_brs(Path),false;true),
        !.
unload_old_brs(Path) :-
        true.

unload_brs(BrsPath) :-
        pd_file(BrsID, _, BrsPath, _, _, _),
        pd_source2xcoff(SrcID, BrsID, _),
        count(group(ID,pd_source2xcoff(SrcID,ID,_)),C),
        C =:= 1,
        remove_src(SrcID).
unload_brs(BrsPath) :-
        retract(pd_file(BrsID,_,BrsPath,_,_,_)),
        retract(pd_source2xcoff(_, BrsID, _)).
unload_brs(_) :-
%       \+ (pd_file(ID,_,_,_,_,_), pd_source2xcoff(_,ID,_)),
%        remove_all_src.
        pd_file(ID,_,Path,_,_,_),
        \+   pd_source2xcoff(_,ID,_),
        \+   reachable_to_brs(ID),
        unsconsult(Path).

reachable_to_brs(ID) :-
        reachable_to_brs(ID,[ID]).

reachable_to_brs(ID,L) :-
        pd_include(PID,ID,_),
        \+ is_included(PID,L),
        reachable_to_brs(PID,[PID|L]).

reachable_to_brs(ID,_) :-
        pd_source2xcoff(ID,_,_).


unload_brs(_) :-
        pd_directory(_,_,_,PID),
        \+ pd_directory(PID,_,_,_),
        remove_dir(PID).

remove_src(ID) :-
        pd_include(ID, CID,_),
        count(group(PID,pd_include(PID,CID,_)),C),
        C =:= 1,
        remove_src(CID).

remove_src(ID) :-
        pd_file(ID, _, Path, _, _, _),
        unsconsult(Path).

remove_all_src :-
        pd_file(_, _, Path, _, _, _),
        unsconsult(Path),
        fail.
remove_all_src.

remove_dir(ID) :-
        pd_directory(CID,_,_,ID),
        remove_dir(CID).
remove_dir(ID) :-
        \+ pd_directory(_,_,_,ID),
        \+ pd_file(_,_,_,_,_,ID),
        retract(pd_directory(ID,_,_,_)).

find_file(PathExp, File) :-
        normalize_filename(PathExp, Path),
        find_file(Path, '.', File).

find_file(_, '\', '\').

find_file(PathExp, CurrentDir, File) :-
        divpath(PathExp, PathExp2, FileExp),
        !,
        find_file(PathExp2, PathExp2, Dir),
        dir(Dir, Member, File),
        wildcard_match(Member,FileExp).

find_file(PathExp, CurrentDir, File) :-
        dir(CurrentDir, Member, File),
        wildcard_match(Member,PathExp).

normalize_filename(Name, Norm) :-
        divpath(Name, Parent, '.'),
        !,
        normalize_filename(Parent, Norm).

normalize_filename(Name, Norm) :-
        divpath(Name, Parent, '..'),
        !,
        normalize_filename(Parent, NormParent),
        divpath(NormParent, Norm, _).

normalize_filename(Name, Norm) :-
        divpath(Name, Parent, Child),
        !,
        normalize_filename(Parent, NormParent),
        divpath(Norm, NormParent, Child).

normalize_filename('..', Norm) :-
        !,
        divpath('.', Norm, _).

normalize_filename('.', Norm) :-
        !,
        divpath('.', Parent, Current),
        divpath(Norm, Parent, Current).

normalize_filename('\', '\') :-
        !.

normalize_filename(Name, Norm) :-
        divpath('.', Parent, Member),
        divpath(Current, Parent, Member),
        divpath(Norm, Current, Name).

%%% end local

%%%%%% Scope Queries

local_scopes(ID, Name, QName, Kind) :-
        generic(ID, Name, QName, Kind),
        group(ID, (
                (
                        pd_defined(ID, BlockID, _, _)
                        ;
                        pd_declared(ID, BlockID, _, _)
                ),
                \+ pd_file(BlockID,_,_,_,_,_),
                \+ pd_class(BlockID,_,_)
        )).

class_scopes(ID, Name, QName, Kind) :-
        pd_member(_, ID, _, _),
        generic(ID, Name, QName, Kind).

file_scopes(ID, Name, QName, Kind) :-
        group(ID, (
                pd_defined(ID, BlockID, _, _),
                pd_file(BlockID, _, _, _, _, _),
                \+ pd_member(_,ID,_,_)
                ;
                pd_declared(ID, BlockID, _, _),
                pd_file(BlockID, _, _, _, _, _)
                ;
                pd_macro(ID,_,_)
        )),
        generic(ID, Name, QName, Kind).

all_globals(ID, Name, QName, Kind) :-
        file_scopes(ID, Name, QName, Kind).
all_globals(ID, Name, QName, Kind) :-
        pd_member(ClassID, ID, public, _),
        \+ local(ClassID),
        generic(ID, Name, QName, Kind).

all_names(ID, Name, QName, Kind) :-
        generic(ID, Name, QName, Kind).

%%%%%% Directory Queries

directory_list(FID, ID, Name, QName, directory) :-
        pd_directory(ID, Name, QName, FID).
directory_list(FID, ID, Name, QName, file) :-
        pd_file(ID, Name, QName, _, _, FID),
        \+ pd_source2xcoff(_, ID, _).

%%%%%% File Queries

file_declaration(BaseFID, ID, Name, QName, Kind, Type) :-
        group(ID, (
                (
                        Type = direct,
                        BaseFID = FID
                        ;
                        Type = indirect,
                        file_all_includes(BaseFID, FID, _, _, _)
                ),
                (
                        pd_declared(ID, FID, _, _),file(FID)
                        ;
                        pd_defined(ID, FID, _, _), file(FID)
                )
        )),
        \+ pd_function(ID,_,_,_,_,_,_,_,_,compiler_generated, _),
        generic(ID, Name, QName, Kind).

file_direct_includes(FID, ID, Name, QName, file) :-
        file_includes(1, FID, ID, Name, QName, _).

file_direct_inclusions(FID, ID, Name, QName, file) :-
        file_inclusions(1, FID, ID, Name, QName, _).

file_all_includes(FID, ID, Name, QName, file) :-
        file_includes(10000, FID, ID, Name, QName, _).

file_all_inclusions(FID, ID, Name, QName, file) :-
        file_inclusions(10000, FID, ID, Name, QName, _).

file_includes(L, FID, ID, Name, QName, file) :-
        group(ID, file_includes_aux(L, FID, ID, Name, QName, [FID])).

file_inclusions(L, FID, ID, Name, QName, file) :-
        group(ID, file_inclusions_aux(L, FID, ID, Name, QName, [FID])).

% local

file_includes_aux(L, FID, ID, Name, QName, H) :-
        pd_include(FID, ID, _),
        \+ is_member(ID, H),
        pd_file(ID, Name, QName, _, _, _)
        ;
        L > 1, L1 is L - 1,
        pd_include(FID, TID, _),
        \+ is_member(TID, H),
        file_includes_aux(L1, TID, ID, Name, QName, [TID|H]).

file_inclusions_aux(L,FID, ID, Name, QName, H) :-
        pd_include(ID, FID, _),
        \+ is_member(ID, H),
        pd_file(ID, Name, QName, _, _, _)
        ;
        L > 1, L1 is L - 1,
        pd_include(TID, FID, _),
        \+ is_member(TID, H),
        file_inclusions_aux(L1, TID, ID, Name, QName, [TID|H]).

% end local

%%%%%% Class Queries

class_declaration(FID, ID, Name, QName, Type, Access) :-
        group(ID, (
                (
                        pd_declared(ID, FID, _, _)
                        ;
                        pd_defined(ID, FID, _, _)
                ),
                pd_member(FID, ID, Access, _),
                generic(ID, Name, QName, Type)
                ;
                pd_friend(FID, ID, _, _,_),
                Access = friend,
                generic(ID, Name, Name1, Type),
                concat('friend ', Name1, QName)
        )).

class_members(FID, ID, Name, QName, Type, immediate, BaseAccess, MemAccess) :-
        pd_member(FID, ID, MemAccess, _),
        generic(ID, Name, QName, Type).

class_members(FID, ID, Name, QName, Type, base, BaseAccess, MemAccess) :-
        (
                class_base_classes_aux(10000,FID,IID,_,BaseAccess,false)
                ;
                group(IID,class_base_classes_aux(10000,FID,IID,_,BaseAccess,true))
        ),
        pd_member(IID, ID, MemAccess, _),
        generic1(ID, Name, Name1, Type),
        (
                MemAccess = public,
                (
                        BaseAccess = public,
                        concat('public:    ', Name1, QName)
                        ;
                        BaseAccess = protected,
                        concat('protected: ', Name1, QName)
                        ;
                        BaseAccess = private,
                        concat('private:   ', Name1, QName)
                )
                ;
                MemAccess = protected,
                (
                        BaseAccess = public,
                        concat('protected:    ', Name1, QName)
                        ;
                        BaseAccess = protected,
                        concat('protected: ', Name1, QName)
                        ;
                        BaseAccess = private,
                        concat('private:   ', Name1, QName)
                )
                ;
                MemAccess = private,
                concat('private:   ', Name1, QName)
        ).

class_friends(FID, ID, Name, QName, Type) :-
        pd_friend(FID, ID, _, _, _),
        generic(ID, Name, QName, Type).

class_friendships(FID, ID, Name, Name, class) :-
        pd_friend(ID, FID, _, _, _),
        pd_class(ID, Name, _).

class_direct_base_classes(FID, ID, Name, QName, class) :-
        class_base_classes(1, FID, ID, Name, QName, _).

class_direct_derived_classes(FID, ID, Name, QName, class) :-
        class_derived_classes(1, FID, ID, Name, QName, _).

class_all_base_classes(FID, ID, Name, QName, class) :-
        class_base_classes(10000, FID, ID, Name, QName, _).

class_all_derived_classes(FID, ID, Name, QName, class) :-
        class_derived_classes(10000, FID, ID, Name, QName, _).

class_base_classes(L, FID, ID, Name, Name, class) :-
        group(ID, class_base_classes_aux(L, FID, ID, Name, _, _)).

class_derived_classes(L, FID, ID, Name, Name, class) :-
        group(ID, class_derived_classes_aux(L, FID, ID, Name)).

class_implementation_file(FID, ID, Name, QName, file) :-
        group(ID, (
                defined(FID, ID, _, _)
                ;
                pd_member(FID, MID, _, _),
                defined(MID, ID, _, _)
        )),
        pd_file(ID, Name, QName, _, _, _).

class_template_instantiations(FID, ID, Name, QName, class) :-
        pd_instantiated(ID, FID),
        generic(ID, Name, QName, _).

% local

class_base_classes_aux(L, FID, ID, Name, BaseAccess, Virtual) :-
        pd_inherit(FID, ID, BaseAccess, Virtual, _, _),
        pd_class(ID, Name, _)
        ;
        L > 1, L1 is L - 1,
        pd_inherit(FID, TID, CurrentAccess, CurrentVirtual, _, _),
        class_base_classes_aux(L1, TID, ID, Name, RestAccess, RestVirtual),
        inherit_access_op(CurrentAccess, RestAccess, BaseAccess),
        or_op(CurrentVirtual, RestVirtual, Virtual).

class_derived_classes_aux(L, FID, ID, Name) :-
        pd_inherit(ID, FID, _, _, _, _),
        pd_class(ID, Name, _)
        ;
        L > 1, L1 is L - 1,
        pd_inherit(TID, FID, _, _, _, _),
        class_derived_classes_aux(L1, TID, ID, Name).

% inherit_access_op(+Current, +Rest, -Result)

inherit_access_op(public, public, public) :- !.
inherit_access_op(_     , _     ,private).

% end local

%%%%%% Function Queries

function_direct_calls(FID, ID, Name, QName, function) :-
        function_calls(1, FID, ID, Name, QName, _).

function_direct_callers(FID, ID, Name, QName, function) :-
        function_callers(1, FID, ID, Name, QName, _).

function_all_calls(FID, ID, Name, QName, function) :-
        function_calls(10000, FID, ID, Name, QName, _).

function_all_callers(FID, ID, Name, QName, function) :-
        function_callers(10000, FID, ID, Name, QName, _).

function_calls(L, FID, ID, Name, QName, function) :-
        group(ID, function_calls_aux(L, FID, ID, Name, QName, [])).

function_callers(L, FID, ID, Name, QName, function) :-
        group(ID, function_callers_aux(L, FID, ID, Name, QName, [])).

function_template_instantiations(FID, ID, Name, QName, function) :-
        pd_instantiated(ID, FID),
        generic(ID, Name, QName, _).

% local

function_calls_aux(L, FID, ID, Name, QName, H) :-
        calls(FID, ID),
        \+ is_member(ID, H),
        pd_function(ID, Name, Name1, _, _, _, _, _, _, _, _),
        make_qname(ID, Name1, QName)
        ;
        L > 1, L1 is L - 1,
        calls(FID, TID),
        \+ is_member(TID, H),
        function_calls_aux(L1, TID, ID, Name, QName, [TID|H]).

function_callers_aux(L, FID, ID, Name, QName, H) :-
        calls(ID, FID),
        \+ is_member(ID, H),
        pd_function(ID, Name, Name1, _, _, _, _, _, _, _, _),
        make_qname(ID, Name1, QName)
        ;
        L > 1, L1 is L - 1,
        calls(TID, FID),
        \+ is_member(TID, H),
        function_callers_aux(L1, TID, ID, Name, QName, [TID|H]).

% end local

%%%%%% List Pane Queries

impacted_file(FID, ID, Name, QName, file) :-
        group(ID, ((
                declared(FID, PID, _,_)
                ;
                defined(FID, PID, _,_)
                ;
                used(FID, PID, _, _)
                ;
                pd_instantiated(InstID, FID),
                used(InstID, PID, _, _)
                ),
                include_closure(ID, PID)
        )),
        pd_file(ID, Name, QName, _, _, _).

usage(FID, ID, Name, QName, function) :-
        group(ID, (
                (
                        pd_used(FID, BID, _, _)
                        ;
                        pd_used_lvalue(FID, BID, _, _)
                        ;
                        pd_used_implicit(FID, BID, _, _)
                ),
                block_in_function(BID, ID),
                pd_function(ID, Name, Name1, _, _, _, _, _, _, _, _),
                make_qname(ID, Name1, QName)
        )).

usage(FID, ID, Name, Name, class) :-
        group(ID, (
                (
                        pd_used(FID, ID, _, _);
                        pd_used_lvalue(FID, ID, _, _);
                        pd_used_implicit(FID, ID, _, _)
                ),
                pd_class(ID, Name, _)
        )).

usage(FID, ID, Name, QName, file) :-
        group(ID, (
                (
                        pd_used(FID, ID, _, _);
                        pd_used_lvalue(FID, ID, _, _);
                        pd_used_implicit(FID, ID, _, _)
                ),
                pd_file(ID, Name, QName, _, _, _)
        )).

modifiers(FID, ID, Name, QName, function) :-
        group(ID, (
                (
                        pd_used_lvalue(FID, BID, _, _)
                        ;
                        pd_used(FID, BID, _, _),
                        pd_initializer(FID,_,_)
                ),
                block_in_function(BID, ID),
                pd_function(ID, Name, Name1, _, _, _, _, _, _, _, _),
                make_qname(ID, Name1, QName)
        )).

enumerators(FID, ID, Name, Name, enumerator) :-
        pd_enumerator(ID, Name, FID).


% local

include_closure(PID,CID) :-
        nonvar(PID),!,
        group(CID, include_closure_1(PID, CID, [PID])).
include_closure(PID,CID) :-
        group(PID, include_closure_2(PID, CID, [CID])).

include_closure_1(ID, ID, _).
include_closure_1(PID, CID, H) :-
        group(TID, pd_include(PID, TID,_)),
        \+ is_member(TID, H),
        include_closure_1(TID, CID, [TID|H]).

include_closure_2(ID, ID, _).
include_closure_2(PID, CID, H) :-
        group(TID, pd_include(TID, CID,_)),
        \+ is_member(TID, H),
        include_closure_2(PID, TID, [TID|H]).

% end local

%%%%%%% Graph Pane

%%% directory
% ArcType ::= directory_to_directory | directory_to_file

% local

dir_include(PID, CID, directory_to_file) :-
        pd_file(CID, _, _, _, _, PID),
        \+ pd_source2xcoff(_,CID, _).
dir_include(PID, CID, directory_to_directory) :-
        pd_directory(CID, _, _, PID).

directory_downward2(PID, L, CID, PID, ArcType) :-
        dir_include(PID, CID, ArcType).
directory_downward2(SID, L, CID, PID, ArcType) :-
        L > 1,
        dir_include(SID, SID2,_),
        L2 is L - 1,
        directory_downward2(SID2, L2, CID, PID, ArcType).

directory_downward_expand(ID, right) :-
        directory_downward2(ID, 1, _, _, _), !.
directory_downward_expand(ID, none).

% end local

directory_top_root(SID, '\', Expand) :-
        pd_directory(SID, _, '\',_),
        directory_downward_expand(SID, Expand).

directory_downward(SID, L, CID, CName, PID, PName, ArcType, Expand) :-
        directory_downward2(SID, L, CID, PID, ArcType),
        id_name(CID, CName),
        id_name(PID, PName),
        directory_downward_expand(CID, Expand).

%%% include
% ArcType ::= normal | cycle

% local

file_include_upward(PID, CID, ArcType, History) :-
        group(PID, pd_include(PID, CID, _)),
        include_arc_type(PID, ArcType, History).

file_include_downward(PID, CID, ArcType, History) :-
        group(CID, pd_include(PID, CID, _)),
        include_arc_type(CID, ArcType, History).

include_arc_type(ID, ArcType, History) :-
        var(ArcType),!,
        (
                is_member(ID, History), !,
                ArcType = cycle
                ;
                ArcType = normal
        ).
include_arc_type(ID, cycle, History) :-
        is_member(ID, History).
include_arc_type(ID, normal, History) :-
        not_member(ID, History).

include_upward2(CID, L, CID, PID, ArcType, History) :-
        file_include_upward(PID, CID, ArcType, History).
include_upward2(SID, L, CID, PID, ArcType, History) :-
        L > 1,
        file_include_upward(SID2, SID, normal, History),
        L2 is L - 1,
        include_upward2(SID2, L2, CID, PID, ArcType, [SID2|History]).

include_downward2(PID, L, CID, PID, ArcType, History) :-
        file_include_downward(PID, CID, ArcType, History).
include_downward2(SID, L, CID, PID, ArcType, History) :-
        L > 1,
        file_include_downward(SID, SID2, normal, History),
        L2 is L - 1,
        include_downward2(SID2, L2, CID, PID, ArcType, [SID2|History]).

include_downward_expand(ID, right) :-
        include_downward2(ID, 1, _, _, _, []), !.
include_downward_expand(ID, none).

include_upward_expand(ID, left) :-
        include_upward2(ID, 1, _, _, _, []), !.
include_upward_expand(ID, none).

% end local

include_top_root(SID, Name, Expand) :-
        pd_file(SID,Name,_,_,_,_),
        \+ pd_include(_,SID,_),
        \+ wildcard_match(Name,'*.brs'),
        include_downward_expand(SID, Expand).

include_downward(SID, L, CID, CName, PID, PName, ArcType, Expand) :-
        include_downward2(SID, L, CID, PID, ArcType, [SID]),
        pd_file(CID, CName, _, _, _, _),
        pd_file(PID, PName, _, _, _, _),
        include_downward_expand(CID, Expand).

include_bottom_root(SID, Name, Expand) :-
        pd_file(SID,Name,_,_,_,_),
        \+ pd_include(SID,_,_),
        \+ wildcard_match(Name,'*.brs'),
        include_upward_expand(SID, Expand).

include_upward(SID, L, CID, CName, PID, PName, ArcType, Expand) :-
        include_upward2(SID, L, CID, PID, ArcType, [SID]),
        pd_file(CID, CName, _, _, _, _),
        pd_file(PID, PName, _, _, _, _),
        include_upward_expand(PID, Expand).

%%% inherit
% ArcType ::= [Access|Virtual]
% Access  ::= private | public
% Virtual ::= virtual | no_specifier

% local

inherit_upward2(CID, L, CID, PID, [Access | Virtual]) :-
        pd_inherit(CID, PID, Access, IsVirtual, _, _),
        get_virtual(IsVirtual, Virtual).
inherit_upward2(SID, L, CID, PID, ArcType) :-
        L > 1,
        pd_inherit(SID, SID2, _, _, _, _),
        L2 is L - 1,
        inherit_upward2(SID2, L2, CID, PID, ArcType).

inherit_downward2(PID, L, CID, PID, [Access | Virtual]) :-
        pd_inherit(CID, PID, Access, IsVirtual, _, _),
        get_virtual(IsVirtual, Virtual).
inherit_downward2(SID, L, CID, PID, ArcType) :-
        L > 1,
        pd_inherit(SID2, SID, _, _, _, _),
        L2 is L - 1,
        inherit_downward2(SID2, L2, CID, PID, ArcType).

get_virtual(true, virtual).
get_virtual(false,non_virtual).

inherit_upward_expand(ID, left) :-
        inherit_upward2(ID, 1, _, _, _), !.
inherit_upward_expand(ID, none).

inherit_downward_expand(ID, right) :-
        inherit_downward2(ID, 1, _, _, _), !.
inherit_downward_expand(ID, none).

% end local

inherit_top_root(SID, Name, Expand) :-
        pd_class(SID, Name, _),
        \+ pd_inherit(SID,_,_,_,_,_),
        inherit_downward_expand(SID, Expand).

inherit_downward(SID, L, CID, CName, PID, PName, ArcType, Expand) :-
        inherit_downward2(SID, L, CID, PID, ArcType),
        pd_class(CID, CName, _),
        pd_class(PID, PName, _),
        inherit_downward_expand(CID, Expand).

inherit_bottom_root(SID, Name, Expand) :-
        pd_class(SID, Name, _),
        \+ pd_inherit(_,SID,_,_,_,_),
        inherit_upward_expand(SID, Expand).

inherit_upward(SID, L, CID, CName, PID, PName, ArcType, Expand) :-
        inherit_upward2(SID, L, CID, PID, ArcType),
        pd_class(CID, CName, _),
        pd_class(PID, PName, _),
        inherit_upward_expand(PID, Expand).

%%% call
% ArcType ::= normal | cycle

% local

function_call_upward(PID, CID, ArcType, History) :-
        calls(PID, CID),
        call_arc_type(PID, ArcType, History).

function_call_downward(PID, CID, ArcType, History) :-
        calls(PID, CID),
        call_arc_type(CID, ArcType, History).

call_arc_type(ID, ArcType, History) :-
        var(ArcType), !,
        (
                is_member(ID, History), !,
                ArcType = cycle
                ;
                ArcType = normal
        ).
call_arc_type(ID, cycle, History) :-
        is_member(ID, History).
call_arc_type(ID,  normal, History) :-
        not_member(ID, History).

call_upward2(CID, L, CID, PID, ArcType, History) :-
        function_call_upward(PID, CID, ArcType, History).
call_upward2(SID, L, CID, PID, ArcType, History) :-
        L > 1,
        function_call_upward(SID2, SID, normal, History),
        L2 is L - 1,
        call_upward2(SID2, L2, CID, PID, ArcType, [SID2|History]).

call_downward2(PID, L, CID, PID, ArcType, History) :-
        function_call_downward(PID, CID, ArcType, History).
call_downward2(SID, L, CID, PID, ArcType, History) :-
        L > 1,
        function_call_downward(SID, SID2, normal, History),
        L2 is L - 1,
        call_downward2(SID2, L2, CID, PID, ArcType,[SID2|History]).

call_upward_expand(ID, left) :-
        call_upward2(ID, 1, _, _, _, []), !.
call_upward_expand(ID, none).

call_downward_expand(ID, right) :-
        call_downward2(ID, 1, _, _, _, []), !.
call_downward_expand(ID, none).

% end local

call_top_root(SID,Name, Expand) :-
        pd_function(SID,Name,_,_,_,_,_,_,_,_, _),
        (
                \+ pd_used(SID,_,_,_),
                \+ pd_used_implicit(SID,_,_,_),
                \+ pd_used_lvalue(SID,_,_,_)
        ),
        call_downward_expand(SID, Expand).

call_downward(SID, L, CID, CName, PID, PName, ArcType, Expand):-
        call_downward2(SID, L, CID, PID, ArcType, [SID]),
        pd_function(CID, CName, _,_,_,_,_,_,_,_, _),
        pd_function(PID, PName, _,_,_,_,_,_,_,_, _),
        call_downward_expand(CID, Expand).

call_bottom_root(SID,Name, Expand) :-
        pd_function(SID,Name,_,_,_,_,_,_,_,_, _),
        (
                \+ pd_used(_,SID,_,_),
                \+ pd_used_implicit(_,SID,_,_),
                \+ pd_used_lvalue(_,SID,_,_)
        ),
        call_upward_expand(SID, Expand).

call_upward(SID, L, CID, CName, PID, PName, ArcType, Expand) :-
        call_upward2(SID, L, CID, PID, ArcType, [SID]),
        pd_function(CID, CName, _,_,_,_,_,_,_,_, _),
        pd_function(PID, PName, _,_,_,_,_,_,_,_, _),
        call_upward_expand(PID, Expand).

%%% program_structure
% ArcType ::=
%       directory_to_file |
%       directory_to_directory |
%       file_to_class |
%       file_to_struct |
%       file_to_union |
%       file_to_function |
%       file_to_variable |
%       file_to_enumeration_tag |
%       file_to_macro |
%       file_to_typedef |
%       file_to_template |
%       class_to_class |
%       class_to_struct |
%       class_to_union |
%       class_to_function |
%       class_to_variable |
%       class_to_enumeration_tag |
%       class_to_macro |
%       class_to_typedef |
%       class_to_template |
%       function_to_variable |
%       function_to_class |
%       enumeration_tag_to_enumerator

% local

include(PID, CID, directory_to_file) :-
        pd_file(CID, _, _, _, _, PID),
        \+ pd_source2xcoff(_,CID, _).

include(PID, CID, directory_to_directory) :-
        pd_directory(CID, _, _, PID).

include(FID,ID,T) :-
        pd_defined(ID,FID,_,_),
        \+ pd_member(_,ID,_,_),
        pd_file(FID,_,_,_,_,_),
        (
         pd_class(ID,_,class),                  T = file_to_class;
         pd_class(ID,_,struct),                 T = file_to_struct;
         pd_class(ID,_,union),                  T = file_to_union;
         pd_class(ID,_,static_union),           T = file_to_union;
         pd_function(ID,_,_,_,_,_,_,_,_,_, _),  T = file_to_function;
         pd_variable(ID,_,_,_,_,_),             T = file_to_variable;
         pd_enumeration_tag(ID,_),              T = file_to_enumeration_tag;
         pd_macro(ID,_,_),                      T = file_to_macro;
         pd_typedef(ID,_,_),                    T = file_to_typedef;
         pd_template(ID,_,_,_),                 T = file_to_template
        ).

include(CID,ID,T) :-
        pd_member(CID,ID,_,_),
        (
         pd_class(ID,_,class),                  T = class_to_class;
         pd_class(ID,_,struct),                 T = class_to_struct;
         pd_class(ID,_,union),                  T = class_to_union;
         pd_class(ID,_,static_union),                   T = class_to_union;
         pd_function(ID,_,_,_,_,_,_,_,_,_, _),  T = class_to_function;
         pd_variable(ID,_,_,_,_,_),             T = class_to_variable;
         pd_enumeration_tag(ID,_),              T = class_to_enumeration_tag;
         pd_typedef(ID,_,_),                    T = class_to_typedef;
         pd_template(ID,_,_,_),                 T = class_to_template
        ).

include(TemplateID,ID, template_to_instance) :-
        pd_instantiated(ID, TemplateID).

include(EnumID,ID, enumeration_tag_to_enumerator) :-
        pd_enumerator(ID, _, EnumID).

include(FuncID,ID,function_to_class) :-
        nonvar(FuncID),!,
        pd_function(FuncID,_,_,_,_,_,_,_,_,_, _),
        local(ID,FuncID),
        (
                pd_class(ID,_,_),
                T = function_to_class
                ;
                pd_variable(ID,_,_,_,_,_),
                T = function_to_variable
        ),
        \+ pd_member(_,ID,_,_)
        ;
        var(FuncID), !,
        (
                pd_class(ID,_,_),
                T = function_to_class
                ;
                pd_variable(ID,_,_,_,_,_),
                T = function_to_variable
        ),
        \+ pd_member(_,ID,_,_),
        local(ID,FuncID),
        pd_function(FuncID,_,_,_,_,_,_,_,_,_, _).

program_structure_downward2(PID, L, CID, PID, ArcType) :-
        include(PID, CID, ArcType).
program_structure_downward2(SID, L, CID, PID, ArcType) :-
        L > 1,
        include(SID, SID2, _),
        L2 is L - 1,
        program_structure_downward2(SID2, L2, CID, PID, ArcType).

program_structure_downward_expand(ID, right) :-
        program_structure_downward2(ID, 1, _, _, _), !.
program_structure_downward_expand(ID, none).

% end local

program_structure_top_root(SID, Name, Expand) :-
        directory_top_root(SID, Name,_),
        program_structure_downward_expand(SID, Expand).

program_structure_downward(SID, L, CID, CName, PID, PName, ArcType, Expand) :-
        program_structure_downward2(SID, L, CID, PID, ArcType),
        id_name(CID, CName),
        id_name(PID, PName),
        program_structure_downward_expand(CID, Expand).

%%%%%%% Text Pane

whatis(InName, OutName, Line, Column, FileID, 0, keyword, use) :-
        atom(InName),
        shorten(InName, OutName),
        system_keyword(OutName),
        !.

whatis(_, QName, Line, Column, FileID, ID, Kind, definition) :-
        defined(ID, FileID, Line, Column),
        interface_string(ID, QName, Kind).

whatis(_, QName, Line, Column, FileID, ID, Kind, declaration) :-
        declared(ID, FileID, Line, Column),
        interface_string(ID, QName, Kind).

whatis(_, QName, Line, Column, FileID, ID, Kind, use) :-
        group(ID,(
                (
                        pd_used(ID,BID,Line,Column)
                        ;
                        pd_used_lvalue(ID,BID,Line,Column)
%                       ;
%                       pd_used_implicit(ID,BID,Line,Column)
                ),
                block_in_file(BID, FileID)
        )),
        interface_string(ID,QName, Kind).
whatis(_, QName, Line, Colum, FileID, ID, class_template, use) :-
        group(ID, (
                used(InsID,FileID,Line,Colum),
                pd_instantiated(InsID,ID),
                pd_template(ID,_,class,QName)
        )).
whatis(_, QName, Line, Colum, FileID, ID, function_template, use) :-
        group(ID, (
                used(InsID,FileID,Line,Colum),
                pd_instantiated(InsID,ID),
                pd_template(ID,_,function,QName)
        )).

shorten(InName, OutName) :-
        name(InName, InNameList),
        remove_head(InNameList,Tmp),
        remove_tail(Tmp,OutNameList),
        name(OutName, OutNameList).

remove_head([X|I],O) :-
        punctuation(X), !,
        remove_head(I,O).
remove_head(L,L).

remove_tail([X|I],O) :-
        punctuation(X), !,
        remove_tail1(I,O).
remove_tail([],[]) :- !.
remove_tail([X|I],[X|O]) :-
        remove_tail(I,O).

remove_tail1([],[]) :- !.
remove_tail1([X|I],O) :-
        punctuation(X),
        remove_tail1(I,O).

punctuation(X) :- punc_char(C), name(C,[X]).

:- db_define(punc_char/1, [1]).
punc_char(' ').
punc_char('     ').   % tab
punc_char('(').
punc_char(')').
punc_char('{').
punc_char('}').
punc_char('[').
punc_char(']').
punc_char(';').
punc_char(':').
punc_char(',').

% ARM p.7
:- db_define(system_keyword/1, [1]).
system_keyword('asm').
system_keyword('auto').
system_keyword('break').
system_keyword('case').
system_keyword('catch').
system_keyword('char').
system_keyword('class').
system_keyword('const').
system_keyword('continue').
system_keyword('default').
system_keyword('delete').
system_keyword('do').
system_keyword('double').
system_keyword('else').
system_keyword('enum').
system_keyword('extern').
system_keyword('float').
system_keyword('for').
system_keyword('friend').
system_keyword('goto').
system_keyword('if').
system_keyword('inline').
system_keyword('int').
system_keyword('long').
system_keyword('new').
system_keyword('operator').
system_keyword('private').
system_keyword('protected').
system_keyword('public').
system_keyword('register').
system_keyword('return').
system_keyword('short').
system_keyword('signed').
system_keyword('sizeof').
system_keyword('static').
system_keyword('struct').
system_keyword('switch').
system_keyword('template').
system_keyword('this').
system_keyword('throw').
system_keyword('try').
system_keyword('typedef').
system_keyword('union').
system_keyword('unsigned').
system_keyword('virtual').
system_keyword('void').
system_keyword('volatile').
system_keyword('while').

%system_keyword('#define').
%system_keyword('#include').
%system_keyword('#line').
%system_keyword('#undef').
%system_keyword('#if').
%system_keyword('#ifdef').
%system_keyword('#ifndef').
%system_keyword('#elif').
%system_keyword('#else').
%system_keyword('#error').
%system_keyword('#pragma').
%system_keyword('#').

%%%

locate_defined(ID,FileID,Path,Line,Col) :-
        pd_function(ID,_,_,_,_,_,_,_,_,compiler_generated, _), !, fail.
locate_defined(ID,FileID,Path,Line,Col) :-
        defined(ID,FileID,Line,Col),
        pd_file(FileID,_,Path,_,_,_).

locate_declared(ID,FileID,Path,Line,Col) :-
        pd_function(ID,_,_,_,_,_,_,_,_,compiler_generated, _), !, fail.
locate_declared(ID,FileID,Path,Line,Col) :-
        declared(ID,FileID,Line,Col),
        pd_file(FileID,_,Path,_,_,_).
locate_declared(ID,FileID,Path,Line,Col) :-
        defined(ID,FileID,Line,Col),
        pd_file(FileID,_,Path,_,_,_).

locate_used(ID,FileID,Path,Line,Col) :-
        group([Line,Col,FileID],(
                (
                        pd_used(ID,BID,Line,Col)
                        ;
                        pd_used_lvalue(ID,BID,Line,Col)
                        ;
                        pd_used_implicit(ID,BID,Line,Col)
                ),
                block_in_file(BID, FileID)
        )),
        pd_file(FileID,_,Path,_,_,_).
locate_used(TID,FileID,Path,Line,Col) :-
        pd_instantiated(ID,TID),
        used(ID,FileID,Line,Col),
        pd_file(FileID,_,Path,_,_,_).
locate_used(ID,FileID,Path,Line,1) :-   % file
        pd_include(FileID, ID, Line),
        pd_file(FileID,_,Path,_,_,_).

locate_contents(FileID,FileID,Path,1,1) :-
        pd_file(FileID,_,Path,_,_,_).

locate_all_files(FileID, Path, 1, 1) :-
        pd_file(FileID,_,Path,_,_,_),
        \+ pd_source2xcoff(_,FileID,_).

locate_files(DirID, FileID, Path, 1, 1) :-
        pd_file(FileID,_,Path,_,_,DirID),
        \+ pd_source2xcoff(_,FileID,_).

%%%%%%% generic

id_name(ID, Name) :-
        generic1(ID,Name,_,_).

id_kind(ID,Type) :-
        generic1(ID, _, _, Type).

interface_string(ID, Name, Type) :-
        generic1(ID, N, QN, Type),
        (
                (
                        Type = directory
                        ;
                        Type = file
                        ;
                        Type = function
                        ;
                        Type = function_template
                        ;
                        Type = class_template
                ),
                Name = QN
                ;
                (
                        \+ Type = directory,
                        \+ Type = file,
                        \+ Type = function,
                        \+ Type = function_template,
                        \+ Type = class_template
                ),
                Name = N
        ).

generic(ID, Name, QualifiedName, Type) :-
        generic1(ID, Name, QName, Type),
        make_qname(ID, QName, QualifiedName).

generic1(ID, Name, QName, directory) :-
        pd_directory(ID, Name, QName, _).
generic1(ID, Name, QName, file) :-
        pd_file(ID, Name, QName, _, _, _),
        \+ wildcard_match(Name, '*.brs').
generic1(ID, Name, Name, class) :-
        pd_class(ID, Name, _).
generic1(ID, Name, QName, function) :-
        pd_function(ID, Name, QName, _, _, _, _, _, _, _, _).
generic1(ID, Name, QName, variable) :-
        pd_variable(ID, Name, QName, _, _, _).
generic1(ID, Name, Name, enumeration_tag) :-
        pd_enumeration_tag(ID, Name).
generic1(ID, Name, Name, enumerator) :-
        pd_enumerator(ID, Name,_).
generic1(ID, Name, Name, macro) :-
        pd_macro(ID, Name, _).
generic1(ID, Name, QName, typedef) :-
        pd_typedef(ID,Name, QName).
generic1(ID, Name, QName, function_template) :-
        pd_template(ID, Name, function, QName).
generic1(ID, Name, QName, class_template) :-
        pd_template(ID, Name, class, QName).
generic1(ID, Name, QName, class_template) :-
        pd_template(ID, Name, struct, QName).
generic1(ID, Name, QName, class_template) :-
        pd_template(ID, Name, union, QName).

% make_qname(+, +, -)

make_qname(ID, Name, QName) :-
        pd_member(_, ID, Access, _), !,
        (
                Access = public,    concat('public:    ', Name, QName);
                Access = protected, concat('protected: ', Name, QName);
                Access = private,   concat('private:   ', Name, QName)
        )
        ;
        Name = QName.

% end local

%%% filter

friend(ID) :-
        group(ID,pd_friend(_,ID,_,_,_)).

local(ID) :-
        group(ID, (
                (
                        pd_defined(ID, BlockID, _, _)
                        ;
                        pd_declared(ID, BlockID, _, _)
                )
        )),
        block_in_function(BlockID, PID).

member(ID) :-
        pd_member(_,ID,_, _).

non_member(ID) :-
        \+ pd_member(_,ID,_, _).

global(ID) :-
        \+ pd_member(_,ID,_, _),
        \+ local(ID).

root(ID) :-
        pd_class(ID,_,_),
        \+ pd_inherit(ID, _, _, _, _, _),
        group(ID,pd_inherit(_, ID, _, _, _, _)).

%%% membership

is_member(X, [X|_]) :- !.
is_member(X, [_|Y]) :- is_member(X,Y).

not_member(X,[X|_]) :- !, fail.
not_member(X,[_|Y]) :- !, not_member(X,Y).
not_member(_,[]).

%%% tree

insert_member(Item,Var) :-
        var(Var),!,Var=t(Right,Item,Left).
insert_member(Item,t(Right,Pivot,Left)) :-
        Item < Pivot -> insert_member(Item,Right); insert_member(Item,Left).

lookup_member(Item,Var) :-
        var(Var), !, fail.
lookup_member(Item,t(_,Item,_)) :-
        true, !.
lookup_member(Item,t(Right,Pivot,Left)) :-
        Item < Pivot -> lookup_member(Item,Right); lookup_member(Item,Left).

%%% closure

block_closure(ID1,ID2) :-
        nonvar(ID1) ,!,
        (
                ID2 = ID1
                ;
                pd_defined(ID1,T,_,_),
                block_closure(T,ID2)
        ).
block_closure(ID1,ID2) :-
        nonvar(ID2) ,!,
        (
                ID1 = ID2
                ;
                pd_defined(T,ID2,_,_),
                block_closure(ID1,T)
        ).

block_in_block(ID1,ID2) :-
        block_closure(ID1,ID2).

block_in_function(BID, FID) :-
        nonvar(FID), !,
        pd_function(FID,_,_,_, _,_,_,_,_,_, _),
        block_closure(BID, FID).
block_in_function(BID, FID) :-
        var(FID), !,
        block_closure(BID, FID),
        pd_function(FID,_,_,_, _,_,_,_,_,_, _).

block_in_class(BID, CID) :-
        nonvar(CID), !,
        pd_class(CID, _,_),
        block_closure(BID, CID).
block_in_class(BID, CID) :-
        var(CID), !,
        block_closure(BID, CID),
        pd_class(CID, _,_).

block_in_file(BID, FID) :-
        nonvar(FID), !,
        pd_file(FID, _,_,_,_,_),
        block_closure(BID, FID).
block_in_file(BID, FID) :-
        var(FID), !,
        block_closure(BID, FID),
        pd_file(FID, _,_,_,_,_).

%%% logical operation

or_op(true , _    , true).
or_op(_    , true , true).
or_op(false, false, false).

and_op(false, _    , false).
and_op(_    , false, false).
and_op(true , true , true).

%%% query optimization

eval_and_call(G) :-
        eval_query(G,G1),
        remove_true(G1,G2),
        call(G2).

eval_query(A,A) :- var(A),!.
eval_query((A=A,B),Y) :- !,
        eval_query(B,Y).
eval_query((A,B),(X,Y)) :-  !,
        eval_query(A,X),
        eval_query(B,Y).
eval_query((A=A),true) :- !.
eval_query((A;B),X) :- !,
        (
                eval_query(A,X)
                ;
                eval_query(B,X)
        ).
eval_query(A,A).

remove_true(A,C) :-
        remove_true1(A,B),
        remove_true2(B,C).

remove_true1((true,A),X) :- !,
        remove_true1(A,X).
remove_true1((A,true),A) :- !.
remove_true1((A,B),(A,X)) :- !,
        remove_true1(B,X).
remove_true1(A,A).

remove_true2((A,true),A) :- !.
remove_true2((A,B),(A,X)) :- !,
        remove_true2(B,X).
remove_true2(A,A).


load_pdinterface(done).

