-- script.lua
-- File này được tải khi ứng dụng MFC khởi động

-- C++ Call Lua: Hàm Lua để C++ gọi cho phép Cộng
function LuaAdd(num1, num2)
    return num1 + num2
end

-- C++ Call Lua: Hàm Lua cho phép Nhân
function LuaMultiply(num1, num2)
    return num1 * num2
end

-- C++ Call Lua: Hàm Lua cho phép Trừ
function LuaSubtract(num1, num2)
    return num1 - num2
end

-- C++ Call Lua: Hàm Lua cho phép Chia (có kiểm tra lỗi chia 0)
function LuaDivide(num1, num2)
    -- Kiểm tra chia cho 0 trong Lua
    if num2 == 0 then
        -- Hàm 'error()' của Lua sẽ bị bắt bởi lua_pcall trong C++
        error("Error: The divisor cannot be 0 (Lua)") 
    end
    return num1 / num2
end


-- Lua Call C++: Gọi hàm C++ ngay khi script được tải
ShowMessageBox(" Sclua script loaded successfully! ")