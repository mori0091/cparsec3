/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

FOREACH(impl_Mem, TYPESET(ALL));
FOREACH(impl_Array, TYPESET(ALL));
FOREACH(impl_List, TYPESET(ALL));
FOREACH(impl_Maybe, TYPESET(ALL));

FOREACH(impl_Mem,
        APPLY(Array, TYPESET(ALL)),
        APPLY(List, TYPESET(ALL)),
        APPLY(Maybe, TYPESET(ALL)));

FOREACH(impl_Array,
        APPLY(Array, TYPESET(ALL)),
        APPLY(List, TYPESET(ALL)),
        APPLY(Maybe, TYPESET(ALL)));

FOREACH(impl_List,
        APPLY(Array, TYPESET(ALL)),
        APPLY(List, TYPESET(ALL)),
        APPLY(Maybe, TYPESET(ALL)));

FOREACH(impl_Maybe,
        APPLY(Array, TYPESET(ALL)),
        APPLY(List, TYPESET(ALL)),
        APPLY(Maybe, TYPESET(ALL)));
