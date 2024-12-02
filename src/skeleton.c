#include <stdio.h>
#include <string.h>
#include "defs.h"

#define SV_CLASS_DECL "@SV_CLASS_DECL@"
#define SV_RUN        "@SV_RUN@"
#define SV_CONSTRUCT  "@SV_CONSTRUCT@"

/*  If the skeleton is changed, the banner should be changed so that	*/
/*  the altered version can easily be distinguished from the original.	*/

char *banner[] = {
    "",
    0
};

char *tables[] = {
    "extern int yylhs[];",
    0
};

char *header[] = {
    "static bit  yydebug;           //do I want debug output?",
    "int yynerrs;            //number of errors so far",
    "int yyerrflag;          //was there an error?",
    "int yychar = -1;             //the current working character\n",
    "//###############################################################",
    "// method: debug",
    "//###############################################################",
    "function void debug(string msg);",
    "  if (yydebug)",
    "    $display(msg);",
    "endfunction\n\n",
    "//########## STATE STACK ##########",
    "\n  int statestk[$]; //state stack",
    "//###############################################################",
    "// methods: state stack push,pop,drop,peek",
    "//###############################################################",
    "function void state_push(int state);",
    "  statestk.push_front(state);",
    "endfunction\n",
    "function state_pop();",
    "  return statestk.pop_front();",
    "endfunction\n",
    "function void state_drop(int cnt);",
    "  statestk = statestk[cnt:$];",
    "endfunction\n",
    "static string ascii[] = {\"\\\\0\",\"SOH\",\"STX\",\"ETX\",\"EOT\",\"ENQ\",\"ACK\",\"\\\\a\",\"\\\\b\",\"\\\\t\",\"\\\\n\",\"\\\\v\",\"\\\\f\",\"\\\\r\",\"SO\",\"SI\",",
    "      \"DLE\",\"DC1\",\"DC2\",\"DC3\",\"DC4\",\"NAK\",\"SYN\",\"ETB\",\"CAN\",\"EM\",\"SUB\",\"ESC\",\"FS\",\"GS\",\"RS\",\"US\",",
    "      \"SPACE\",\"!\",\"\\\"\",\"#\",\"$\",\"%\",\"&\",\"’\",\"(\",\")\",\"*\",\"+\",\",\",\"-\",\".\",\"/\",",
    "      \"0\",\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\",\"8\",\"9\",\":\",\";\",\"<\",\"=\",\">\",\"?\",",
    "      \"@\",\"A\",\"B\",\"C\",\"D\",\"E\",\"F\",\"G\",\"H\",\"I\",\"J\",\"K\",\"L\",\"M\",\"N\",\"O\",",
    "      \"P\",\"Q\",\"R\",\"S\",\"T\",\"U\",\"V\",\"W\",\"X\",\"Y\",\"Z\",\"[\",\"\\\\\",\"]\",\"^\",\"_\",",
    "      \"`\",\"a\",\"b\",\"c\",\"d\",\"e\",\"f\",\"g\",\"h\",\"i\",\"j\",\"k\",\"l\",\"m\",\"n\",\"o\",",
    "      \"p\",\"q\",\"r\",\"s\",\"t\",\"u\",\"v\",\"w\",\"x\",\"y\",\"z\",\"{\",\"|\",\"}\",\"~\",\"DEL\"};\n",
    "//###############################################################",
    "// method: init_stacks : allocate and prepare stacks",
    "//###############################################################",
    "function bit init_stacks();",
    "  val_init();",
    "  if (ascii.size() <= 128) init_debug();",
    "  return 1;",
    "endfunction\n",
    0
};

char *body[] = {
    "//###############################################################",
    "// method: dump_stacks : show n levels of the stacks",
    "//###############################################################",
    "function void dump_stacks(int count);",
    "  int i;",
    "  debug($psprintf(\"=index==state====value=  \"));",
    "  for (i=0;i<count;i++)",
    "    debug($psprintf(\" %0d %0d %p\", i, statestk[i], valstk[i]));",
    "  debug(\"======================\");",
    "endfunction\n",
    "//###############################################################",
    "// method: yylexdebug : check lexer state",
    "//###############################################################",
    "function void yylexdebug(int state,int ch);",
    "  string s;",
    "  if (ch < 0) ch=0;",
    "  if (ch <= `YYMAXTOKEN) //check index bounds",
    "    s = yyname[ch];    //now get it",
    "  if (s == \"\")",
    "    s = \"illegal-symbol\";",
    "  debug($psprintf(\"state %0d, reading %0d (%0s)\", state, ch, s));",
    "endfunction\n\n\n",
    "//The following are now global, to aid in error reporting",
    "int yyn;       //next next thing to do",
    "int yym;       //",
    "int yystate;   //current parsing state from state table",
    "string yys;    //current token string",
    "",
    "//###############################################################",
    "// method: yyparse : parse input and execute indicated items",
    "//###############################################################",
    "function int yyparse();",
    "  bit doaction;",
    "  init_stacks();",
    "  yynerrs = 0;",
    "  yyerrflag = 0;",
    "  //yychar = -1;          //impossible char forces a read",
    "  yystate=0;            //initial state",
    "  state_push(yystate);  //save it",
    "  val_push(yylval);     //save empty value",
    "  while (1) begin //until parsing is done, either correctly, or w/error",
    "    doaction=1;",
    "    debug(\"loop\"); ",
    "    //#### NEXT ACTION (from reduction table)",
    "    for (yyn = yydefred[yystate]; yyn == 0; yyn = yydefred[yystate]) begin",
    "      debug($psprintf(\"yyn:%0d  state:%0d  yychar: %s (%0d)\", yyn, yystate, ascii[yychar], yychar));",
    "      if (yychar < 0) begin     //we want a char?",
    "        yychar = yylex();  //get next token",
    "        debug($psprintf(\" next yychar: %s (%0d)\",ascii[yychar], yychar));",
    "        //#### ERROR CHECK ####",
    "        if (yychar < 0) begin   //it it didn't work/error",
    "          yychar = 0;      //change it to default string (no -1!)",
    "          if (yydebug)",
    "            yylexdebug(yystate,yychar);",
    "        end",
    "      end//yychar<0",
    "      yyn = yysindex[yystate];  //get amount to shift by (shift index)",
    "      if (yyn != 0) begin",
    "        yyn += yychar;",
    "        if (yyn >= 0 && yyn <= `YYTABLESIZE && yycheck[yyn] == yychar) begin",
    "          debug($psprintf(\"state %0d, shifting to state %0d\", yystate,yytable[yyn]));",
    "          //#### NEXT STATE ####",
    "          yystate = yytable[yyn];//we are in a new state",
    "          state_push(yystate);   //save it",
    "          val_push(yylval);      //push our lval as the input for next rule",
    "          yychar = -1;           //since we have 'eaten' a token, say we need another",
    "          if (yyerrflag > 0)     //have we recovered an error?",
    "             --yyerrflag;        //give ourselves credit",
    "          doaction=0;            //but don't process yet",
    "          break;                 //quit the yyn=0 loop",
    "        end",
    "      end",
    "      yyn = yyrindex[yystate];  //reduce",
    "      if (yyn != 0) begin",
    "        yyn += yychar;",
    "        if (yyn >= 0 && yyn <= `YYTABLESIZE && yycheck[yyn] == yychar) begin //we reduced!",
    "          debug(\"reduce\");",
    "          yyn = yytable[yyn];",
    "          doaction=1; //get ready to execute",
    "          break;      //drop down to actions",
    "        end else begin",         
    "          execerror(\"Cannot recover from parse error\", yys);",         
    "          $finish;",         
    "        end",
    "      end",
    "      else //ERROR RECOVERY",
    "      begin",
    "        if (yyerrflag==0) begin",
    "          yyerror(\"syntax error\");",
    "          yynerrs++;",
    "        end",
    "        if (yyerrflag < 3) begin //low error count?",
    "          yyerrflag = 3;",
    "          while (1) begin  //do until break",
    "            yyn = yysindex[statestk[0]];",
    "            if (yyn != 0) begin",
    "              yyn += `YYERRCODE;",
    "              if (yyn >= 0 && yyn <= `YYTABLESIZE && yycheck[yyn] == `YYERRCODE) begin",
    "                debug($psprintf(\"state %0d, error recovery shifting to state %0d \", statestk[0], yytable[yyn]));",
    "                yystate = yytable[yyn];",
    "                state_push(yystate);",
    "                val_push(yylval);",
    "                doaction=0;",
    "                break;",
    "              end",
    "            end",
    "            else",
    "            begin",
    "              debug($psprintf(\"error recovery discarding state %0d \", statestk[0]));",
    "              state_pop();",
    "              val_pop();",
    "            end",
    "          end",
    "        end",
    "        else //discard this token",
    "        begin",
    "          if (yychar == 0)",
    "            return 1; //yyabort",
    "          if (yydebug) begin",
    "            yys = \"\";",
    "            if (yychar <= `YYMAXTOKEN) yys = yyname[yychar];",
    "            if (yys == \"\") yys = \"illegal-symbol\";",
    "            debug($psprintf(\"state %0d, error recovery discards token  %s (%0d)\", yystate, ascii[yychar], yys));",
    "          end",
    "          yychar = -1;  //read another",
    "        end",
    "      end  //end error recovery",
    "    end   //yyn=0 loop",
    "    if (!doaction)   //any reason not to proceed?",
    "      continue;      //skip action",
    "    yym = yylen[yyn];          //get count of terminals on rhs",
    "    debug($psprintf(\"state %0d, reducing %0d by rule %0d (%s)\", yystate, yym, yyn, yyrule[yyn]));",
    "    if (yym>0)                 //if count of rhs not 'nil'",
    "      yyval = valstk[yym-1]; //get current semantic value",
    "    yyval = dup_yyval(yyval);  //duplicate yyval if ParserVal is used as semantic value",
    "    case(yyn)",
    "    //########## USER-SUPPLIED ACTIONS ##########",
    0
};

char *trailer[] = {
    "    //########## END OF USER-SUPPLIED ACTIONS ##########",
    "    endcase //case",
    "    //#### Now let's reduce... ####",
    "    debug(\"reduce\");",
    "    state_drop(yym);             //we just reduced yylen states",
    "    yystate = statestk[0];     //get new state",
    "    val_drop(yym);               //corresponding value drop",
    "    yym = yylhs[yyn];            //select next TERMINAL(on lhs)",
    "    if (yystate == 0 && yym == 0) begin//done? 'rest' state and at first TERMINAL",
    "      debug($psprintf(\"After reduction, shifting from state 0 to state %0d\",`YYFINAL));",
    "      yystate = `YYFINAL;         //explicitly say we're done",
    "      state_push(`YYFINAL);       //and save it",
    "      val_push(yyval);           //also save the semantic value of parsing",
    "      if (yychar < 0) begin      //we want another character?",
    "        yychar = yylex();        //get next character",
    "        debug($psprintf(\" next yychar: %s (%0d)\",ascii[yychar], yychar));",
    "        if (yychar<0) yychar=0;  //clean, if necessary",
    "        if (yydebug)",
    "          yylexdebug(yystate,yychar);",
    "      end",
    "      if (yychar == 0)          //Good exit (if lex returns 0 ;-)",
    "         break;                 //quit the loop--all DONE",
    "    end//if yystate",
    "    else                        //else not done yet",
    "    begin                         //get next state and push, for next yydefred[]",
    "      yyn = yygindex[yym];      //find out where to go",
    "      if (yyn != 0) begin",
    "        yyn += yystate;",
    "        if (yyn >= 0 && yyn <= `YYTABLESIZE && yycheck[yyn] == yystate)",
    "          yystate = yytable[yyn]; //get new state",
    "        else",
    "          yystate = yydgoto[yym]; //else go to new defred",
    "      end",
    "      else",
    "        yystate = yydgoto[yym]; //else go to new defred",
    "      debug($psprintf(\"after reduction, shifting from state %0d to state %0d\", statestk[0], yystate));",
    "      state_push(yystate);     //going again, so push state & val...",
    "      val_push(yyval);         //for next action",
    "    end",
    "  end//main loop",
    "  return 0;//yyaccept!!",
    "endfunction",
    "//## end of method parse() ######################################",
    "\n",
    "//###############################################################",
    "task init_debug();",
    "  automatic int idx = `YYMAXTOKEN - `YYNTOKENS;",
    "  ascii = new [`YYNTOKENS + 128] (ascii);",
    "  for (int l = 128; l <= 128 + `YYNTOKENS; l++, idx++) begin",
    "    ascii[l] = yyname[idx];",
    "  end",
    "endtask",
    "\n",
    0
};

void write_section(char **section)
{
    int i;
    FILE *fp;

    fp = code_file;
    for (i = 0; section[i]; ++i) {
        ++outline;
        if (strcmp(section[i], SV_CLASS_DECL) == 0) {
            if (svclass_name && strlen(svclass_name) > 0)
                fprintf(fp, "class %s", svclass_name);
            else
                fprintf(fp, "class Parser");
            if (svextend_name && strlen(svextend_name) > 0)
                fprintf(fp, "             extends %s", svextend_name);
            fprintf(fp, ";\n");
        } else if (strcmp(section[i], SV_RUN) == 0) {
            if (svrun) {
                fprintf(fp, "  /**\n");
                fprintf(fp, "   * A default run method, used for operating this parser\n");
                fprintf(fp, "   * object in the background.  It is intended for extending Thread\n");
                fprintf(fp, "   * or implementing Runnable.  Turn off with -SVnorun .\n");
                fprintf(fp, "   */\n");
                fprintf(fp, "  function void run();\n");
                fprintf(fp, "    yyparse();\n");
                fprintf(fp, "  endfunction\n");
                fprintf(fp, "\n");
            } else {
                fprintf(fp, "//## The -SVnorun option was used ##\n");
            }
        } else if (strcmp(section[i], SV_CONSTRUCT) == 0) {
            if (svconstruct) {
                fprintf(fp, "  /**\n");
                fprintf(fp, "   * Default constructor.\n");
                fprintf(fp, "   */\n");
                fprintf(fp, "  function %s();\n", svclass_name);
                fprintf(fp, "    //nothing to do\n");
                fprintf(fp, "  endfunction\n");
                fprintf(fp, "\n");
            } else {
                fprintf(fp, "//## The -SVnoconstruct option was used ##\n");
            }
        } else
            fprintf(fp, "  %s\n", section[i]);
    }
}
