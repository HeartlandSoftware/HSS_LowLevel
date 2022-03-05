#pragma once

#if __has_include(<experimental/propagate_const>)
#include <experimental/propagate_const>
#else //__has_include(<experimental/propagate_const>)
//if the system doesn't have the experimental package here is a third party implementation
//https://github.com/jbcoe/propagate_const/blob/master/propagate_const.h
#include <propagate_const.h>
#endif
