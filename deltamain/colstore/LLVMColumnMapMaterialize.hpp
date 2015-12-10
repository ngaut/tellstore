/*
 * (C) Copyright 2015 ETH Zurich Systems Group (http://www.systems.ethz.ch/) and others.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contributors:
 *     Markus Pilman <mpilman@inf.ethz.ch>
 *     Simon Loesing <sloesing@inf.ethz.ch>
 *     Thomas Etter <etterth@gmail.com>
 *     Kevin Bocksrocker <kevin.bocksrocker@gmail.com>
 *     Lucas Braun <braunl@inf.ethz.ch>
 */

#pragma once

#include <util/LLVMBuilder.hpp>

#include <crossbow/string.hpp>

#include <cstddef>
#include <string>

namespace tell {
namespace store {
namespace deltamain {

class ColumnMapContext;

/**
 * @brief Helper class creating the column map materialize function
 */
class LLVMColumnMapMaterializeBuilder : private FunctionBuilder {
public:
    using Signature = void (*) (
            const char* /* page */,
            uint64_t /* idx */,
            char* /* destData */,
            uint64_t /* size */);

    static const std::string FUNCTION_NAME;

    static void createFunction(const ColumnMapContext& context, llvm::Module& module, llvm::TargetMachine* target) {
        LLVMColumnMapMaterializeBuilder builder(context, module, target);
        builder.build();
    }

private:
    static constexpr size_t page = 0;
    static constexpr size_t idx = 1;
    static constexpr size_t data = 2;
    static constexpr size_t size = 3;

    static llvm::Type* buildReturnTy(llvm::LLVMContext& context) {
        return llvm::Type::getVoidTy(context);
    }

    static std::vector<std::pair<llvm::Type*, crossbow::string>> buildParamTy(llvm::LLVMContext& context) {
        return {
            { llvm::Type::getInt8Ty(context)->getPointerTo(), "page" },
            { llvm::Type::getInt64Ty(context), "idx" },
            { llvm::Type::getInt8Ty(context)->getPointerTo(), "data" },
            { llvm::Type::getInt64Ty(context), "size" }
        };
    }

    LLVMColumnMapMaterializeBuilder(const ColumnMapContext& context, llvm::Module& module, llvm::TargetMachine* target);

    void build();

    const ColumnMapContext& mContext;

    llvm::StructType* mMainPageStructTy;
    llvm::StructType* mHeapEntryStructTy;
};

} // namespace deltamain
} // namespace store
} // namespace tell
