#ifndef SYNTAX_PARSER_H
#define SYNTAX_PARSER_H

#include <Arduino.h>
#include "tool.h"

class SyntaxEngine {
public:
    void execute(String cmd) {
        cmd.trim();
        if (cmd.startsWith("( (") ) { // 處理複合指令 ( (cmd1) (cmd2) )
            parseComposite(cmd);
        } else {
            parseSingle(cmd);
        }
    }

private:
    void parseSingle(String cmd) {
        if (!cmd.startsWith("(") || !cmd.endsWith(")")) return;
        String content = cmd.substring(1, cmd.length() - 1);
        content.trim();

        // 處理 IF 條件句: ( if ( cond TMP > 25 ) then ( do ... ) )
        if (content.startsWith("if")) {
            int thenIdx = content.indexOf("then");
            String condPart = content.substring(content.indexOf("("), thenIdx);
            String actionPart = content.substring(content.indexOf("(", thenIdx));
            
            if (evaluateCondition(condPart)) {
                execute(actionPart);
            }
        } 
        // 處理動作句: ( do ACTION ... )
        else if (content.startsWith("do")) {
            String sub = content.substring(3);
            processDo(sub);
        }
        // 處理延遲: ( sleep 5 )
        else if (content.startsWith("sleep")) {
            int sec = content.substring(6).toInt();
            delay(sec * 1000);
        }
    }

    void parseComposite(String cmd) {
        // 簡單拆解 ( (a) (b) ) 結構
        int depth = 0;
        int start = 1;
        for (int i = 1; i < cmd.length() - 1; i++) {
            if (cmd[i] == '(') depth++;
            if (cmd[i] == ')') depth--;
            if (depth == 0 && cmd[i] == ')') {
                execute(cmd.substring(start, i + 1));
                start = i + 1;
                while(start < cmd.length() && cmd[start] != '(') start++;
            }
        }
    }

    bool evaluateCondition(String cond) {
        // 解析 ( cond TMP > 25 )
        cond = cond.substring(cond.indexOf("cond") + 5, cond.lastIndexOf(")"));
        cond.trim();
        int space1 = cond.indexOf(" ");
        String sensor = cond.substring(0, space1);
        String op = cond.substring(space1 + 1, space1 + 2);
        float threshold = cond.substring(space1 + 3).toFloat();

        float val = getCurrentSensorValue(sensor);
        if (op == ">") return val > threshold;
        if (op == "<") return val < threshold;
        if (op == "=") return abs(val - threshold) < 0.1;
        return false;
    }

    void processDo(String act) {
        act.trim();
        // 1. LED 類: LEDS OER C 4 或 LEDL OEY T 1
        if (act.startsWith("LEDS") || act.startsWith("LEDL")) {
            char type = act.charAt(3); // 'S' 或 'L'
            String remainder = act.substring(5);
            int s1 = remainder.indexOf(" ");
            String colorCode = remainder.substring(0, s1); // OER, OEG...
            int s2 = remainder.indexOf(" ", s1 + 1);
            int val = remainder.substring(s2 + 1).toInt();
            handleLED(colorCode, String(act[3]), val);
        }
        // 2. 顯示類: LCDSV HUM / LCDS Text
        else if (act.startsWith("LCDSV")) {
            String sensor = act.substring(6);
            displayValue(sensor, getCurrentSensorValue(sensor));
        }
        else if (act.startsWith("LCDS")) {
            displayText(act.substring(5));
        }
        // 3. 蜂鳴器: BUZ
        else if (act.startsWith("BUZ")) {
            runBuzzer();
        }
        // 4. 手機訊息: MOBSV / MOBS
        else if (act.startsWith("MOBSV") || act.startsWith("MOBS")) {
            Serial.println("Mobile Notification: " + act);
        }
    }
};

#endif