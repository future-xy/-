load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "card",
    hdrs = ["Card.h"],
    srcs = ["Card.cpp"],
    visibility = ["//main:__pkg__"],
)

cc_library(
    name = "campus_card",
    hdrs = ["Campus_card.h", "Deposit_card.h", "Binding_card.h"],
    srcs = ["Campus_card.cpp"],
    deps = [":card"],
    visibility = ["//main:__pkg__"],
)

cc_library(
    name = "deposit_card",
    hdrs = ["Deposit_card.h", "Campus_card.h", "Binding_card.h"],
    srcs = ["Deposit_card.cpp"],
    deps = [":card"],
    visibility = ["//main:__pkg__"],
)

cc_library(
    name = "binding_card",
    hdrs = ["Binding_card.h"],
    srcs = ["Binding_card.cpp"],
    deps = [":campus_card", ":deposit_card"],
    visibility = ["//main:__pkg__"],
)