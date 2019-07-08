pipeline {
    agent any 
    stages {
        stage('Static analysis') {
              steps {
                  // Remove old files
                sh 'rm -r html_cppcheck'
                sh 'rm cppcheck.xml'
                sh 'mkdir html_cppcheck'
                  
                // Analyze project
                //sh 'cppcheck --xml --xml-version=2 /var/lib/jenkins/workspace/Sudoku_in_terminal 2> cppcheck.xml'
                sh 'cppcheck --enable=all --inconclusive --xml --xml-version=2 /var/lib/jenkins/workspace/Sudoku_in_terminal 2> cppcheck.xml' 
                
                // Make html reports and publish them on jenkins
                sh 'cppcheck-htmlreport --file=cppcheck.xml --report-dir=html_cppcheck --source-dir=.'
                publishHTML ([       
                    reportName:'static_analysis',
                    reportDir:'html_cppcheck/', 
                    reportFiles:'index.html',   
                    keepAll:false,
                    alwaysLinkToLastBuild:false,
                    allowMissing:false 
                ]) 
              }
        }
        stage('Build') { 
            steps {
                // Compile project
                sh 'gcc -I. main.c sudoku.c -o run'
            }
        }
    }
}
