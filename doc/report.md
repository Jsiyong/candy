1.性能测试报告[单位，每秒]

|Num     |Time        |Samples    |Average   |Median   |90%Line  |95%Line  |99%Line  |Min       |Maximun    |Error%    |Throughput    |Received KB/sec    |Sent KB/sec    |note|
|----    |----        |----       |----      |----     |----     |----     |----     |----      |----       |----      |----          |----               |----           |----|
|1       |2021-03-10  |200000000  |158       |111      |354      |453      |616      |0         |2975       |0         |10761         |10330              |1460           |使用改版的线程池;测试用2000线程/1秒循环一次/循环次数100000/持续运行5小时09分钟；测试接口：/getFolder?path=/|
