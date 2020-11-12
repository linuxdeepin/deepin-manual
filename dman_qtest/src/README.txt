注意：
1,Dbug调试模式下，ut_image_viewer_proxy_test::TEST_F(ut_image_viewer_proxy_test, open) 用例 注释掉
2,ut_argument_parser_test：：parseArguments3，先打开帮助手册，再执行make test 覆盖以下代码            
	    qDebug() << Q_FUNC_INFO << "position_args is empty";
            emit newAppOpen();
            //            QString argName = qApp->arguments().value(0);
            //            qDebug() << "argName:" << argName;

            //            if (argName == QString(kAppProcessName) ||
            //                    argName.endsWith("/" + QString(kAppProcessName))) {
            //                qDebug() << "emit onNewAppOpen";
            //                emit newAppOpen();
            //            }
