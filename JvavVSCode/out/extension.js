"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.deactivate = exports.activate = void 0;
const vscode = require("vscode");
const path = require("path");
const child_process_1 = require("child_process");
function activate(context) {
    console.log('Jvav语言支持已激活');
    // 注册编译命令
    const compileCommand = vscode.commands.registerCommand('jvav.compile', () => {
        const editor = vscode.window.activeTextEditor;
        if (!editor) {
            vscode.window.showErrorMessage('没有打开的文件');
            return;
        }
        const document = editor.document;
        if (document.languageId !== 'jvav') {
            vscode.window.showErrorMessage('当前文件不是Jvav文件');
            return;
        }
        // 保存文件
        document.save().then(() => {
            const filePath = document.fileName;
            const config = vscode.workspace.getConfiguration('jvav');
            const compilerPath = config.get('compilerPath', 'jvavc');
            const optimizationLevel = config.get('optimizationLevel', 'O0');
            const targetPlatform = config.get('targetPlatform', 'wasm');
            // 创建输出通道
            const outputChannel = vscode.window.createOutputChannel('Jvav编译');
            outputChannel.show();
            // 执行编译命令
            const command = `${compilerPath} "${filePath}" -${optimizationLevel} --target=${targetPlatform}`;
            outputChannel.appendLine(`执行命令: ${command}`);
            (0, child_process_1.exec)(command, (error, stdout, stderr) => {
                if (error) {
                    outputChannel.appendLine(`编译错误: ${error.message}`);
                    vscode.window.showErrorMessage(`Jvav编译失败: ${error.message}`);
                    return;
                }
                if (stderr) {
                    outputChannel.appendLine(`编译警告: ${stderr}`);
                }
                outputChannel.appendLine(stdout);
                vscode.window.showInformationMessage('Jvav编译成功');
            });
        });
    });
    // 注册运行命令
    const runCommand = vscode.commands.registerCommand('jvav.run', () => {
        const editor = vscode.window.activeTextEditor;
        if (!editor) {
            vscode.window.showErrorMessage('没有打开的文件');
            return;
        }
        const document = editor.document;
        if (document.languageId !== 'jvav') {
            vscode.window.showErrorMessage('当前文件不是Jvav文件');
            return;
        }
        // 保存文件
        document.save().then(() => {
            const filePath = document.fileName;
            const fileDir = path.dirname(filePath);
            const fileName = path.basename(filePath, '.jvav');
            const config = vscode.workspace.getConfiguration('jvav');
            const compilerPath = config.get('compilerPath', 'jvavc');
            const targetPlatform = config.get('targetPlatform', 'wasm');
            const outputPath = path.join(fileDir, fileName);
            // 创建终端
            const terminal = vscode.window.createTerminal('Jvav运行');
            terminal.show();
            // 先编译，然后运行
            if (targetPlatform === 'wasm') {
                terminal.sendText(`${compilerPath} "${filePath}" --target=wasm && node "${outputPath}.wasm.js"`);
            }
            else {
                terminal.sendText(`${compilerPath} "${filePath}" --target=${targetPlatform} && "${outputPath}"`);
            }
        });
    });
    // 注册构建本地可执行文件命令
    const buildNativeCommand = vscode.commands.registerCommand('jvav.buildNative', () => {
        const editor = vscode.window.activeTextEditor;
        if (!editor) {
            vscode.window.showErrorMessage('没有打开的文件');
            return;
        }
        const document = editor.document;
        if (document.languageId !== 'jvav') {
            vscode.window.showErrorMessage('当前文件不是Jvav文件');
            return;
        }
        // 选择目标平台
        const platforms = ['windows', 'macos', 'linux', 'harmony'];
        vscode.window.showQuickPick(platforms, {
            placeHolder: '选择目标平台'
        }).then(platform => {
            if (!platform)
                return;
            document.save().then(() => {
                const filePath = document.fileName;
                const config = vscode.workspace.getConfiguration('jvav');
                const compilerPath = config.get('compilerPath', 'jvavc');
                const optimizationLevel = config.get('optimizationLevel', 'O0');
                // 创建输出通道
                const outputChannel = vscode.window.createOutputChannel('Jvav构建');
                outputChannel.show();
                // 执行构建命令
                const command = `${compilerPath} "${filePath}" -${optimizationLevel} --target=${platform}`;
                outputChannel.appendLine(`执行命令: ${command}`);
                (0, child_process_1.exec)(command, (error, stdout, stderr) => {
                    if (error) {
                        outputChannel.appendLine(`构建错误: ${error.message}`);
                        vscode.window.showErrorMessage(`Jvav构建失败: ${error.message}`);
                        return;
                    }
                    if (stderr) {
                        outputChannel.appendLine(`构建警告: ${stderr}`);
                    }
                    outputChannel.appendLine(stdout);
                    vscode.window.showInformationMessage(`Jvav已成功构建为${platform}平台可执行文件`);
                });
            });
        });
    });
    // 注册启动REPL命令
    const startReplCommand = vscode.commands.registerCommand('jvav.startRepl', () => {
        const config = vscode.workspace.getConfiguration('jvav');
        const terminalPath = config.get('terminalPath', 'jvav_terminal');
        const targetPlatform = config.get('targetPlatform', 'wasm');
        // 创建终端并启动REPL
        const terminal = vscode.window.createTerminal('Jvav REPL');
        terminal.show();
        terminal.sendText(`${terminalPath} --target=${targetPlatform}`);
    });
    // 语法高亮和语言特性
    const jvavDocSelector = { language: 'jvav', scheme: 'file' };
    // 提供自动完成
    const completionProvider = vscode.languages.registerCompletionItemProvider(jvavDocSelector, {
        provideCompletionItems(document, position) {
            // 获取当前行文本
            const lineText = document.lineAt(position).text;
            const linePrefix = lineText.substr(0, position.character);
            const items = [];
            // 关键字提示
            const keywords = ['if', 'else', 'loop', 'define', 'set', 'return', 'try', 'catch', 'in', 'of'];
            const chineseKeywords = ['如果', '否则', '循环', '定义', '设置', '返回', '尝试', '捕获'];
            // 类型提示
            const types = ['number', 'string', 'boolean', 'array', 'object', 'function'];
            const chineseTypes = ['数字', '字符串', '布尔', '数组', '对象', '函数'];
            // 添加关键字
            [...keywords, ...chineseKeywords].forEach(keyword => {
                const item = new vscode.CompletionItem(keyword, vscode.CompletionItemKind.Keyword);
                items.push(item);
            });
            // 添加类型
            [...types, ...chineseTypes].forEach(type => {
                const item = new vscode.CompletionItem(type, vscode.CompletionItemKind.TypeParameter);
                items.push(item);
            });
            // 添加常用函数
            const functions = ['print', 'parseInt', 'parseFloat', 'toString', 'length', 'ask'];
            const chineseFunctions = ['打印', '转整数', '转浮点', '转字符串', '长度', '询问'];
            [...functions, ...chineseFunctions].forEach(func => {
                const item = new vscode.CompletionItem(func, vscode.CompletionItemKind.Function);
                items.push(item);
            });
            return items;
        }
    });
    // 注册所有命令
    context.subscriptions.push(compileCommand, runCommand, buildNativeCommand, startReplCommand, completionProvider);
}
exports.activate = activate;
function deactivate() {
    console.log('Jvav语言支持已停用');
}
exports.deactivate = deactivate;
//# sourceMappingURL=extension.js.map