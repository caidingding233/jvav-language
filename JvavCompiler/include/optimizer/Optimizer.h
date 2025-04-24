#ifndef JVAV_OPTIMIZER_H
#define JVAV_OPTIMIZER_H

#include "ast/AST.h"
#include <string>
#include <vector>
#include <memory>

namespace jvav {

// 优化器类
class Optimizer {
public:
    Optimizer();
    ~Optimizer();
    
    // 优化AST
    void optimize(std::vector<std::unique_ptr<Stmt>>& ast, int optimizationLevel);
    
private:
    // 将在后续实现
    class OptimizerImpl;
    std::unique_ptr<OptimizerImpl> impl_;
};

} // namespace jvav

#endif // JVAV_OPTIMIZER_H 