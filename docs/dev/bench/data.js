window.BENCHMARK_DATA = {
  "lastUpdate": 1736884079057,
  "repoUrl": "https://github.com/paulmthompson/Whisker-Analysis",
  "entries": {
    "Benchmark": [
      {
        "commit": {
          "author": {
            "email": "pmt10@case.edu",
            "name": "Paul Thompson",
            "username": "paulmthompson"
          },
          "committer": {
            "email": "pmt10@case.edu",
            "name": "Paul Thompson",
            "username": "paulmthompson"
          },
          "distinct": true,
          "id": "fc8694703005fee1dff9346f308e2ed2ce9f09e8",
          "message": "use new github pages branch",
          "timestamp": "2025-01-14T14:33:40-05:00",
          "tree_id": "8502b423809535962c973477cc34750321440a96",
          "url": "https://github.com/paulmthompson/Whisker-Analysis/commit/fc8694703005fee1dff9346f308e2ed2ce9f09e8"
        },
        "date": 1736883301212,
        "tool": "catch2",
        "benches": [
          {
            "name": "IoU Calculation",
            "value": 8.34253,
            "range": "± 964.895",
            "unit": "us",
            "extra": "100 samples\n4 iterations"
          },
          {
            "name": "IoU Relative Calculation",
            "value": 7.28966,
            "range": "± 483.079",
            "unit": "us",
            "extra": "100 samples\n5 iterations"
          },
          {
            "name": "Mask Intersection",
            "value": 30.1676,
            "range": "± 4.70079",
            "unit": "ns",
            "extra": "100 samples\n1033 iterations"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "pmt10@case.edu",
            "name": "Paul Thompson",
            "username": "paulmthompson"
          },
          "committer": {
            "email": "pmt10@case.edu",
            "name": "Paul Thompson",
            "username": "paulmthompson"
          },
          "distinct": true,
          "id": "264d118592213e97aa90c3e26571117dd4b1c00f",
          "message": "add line benchmarks",
          "timestamp": "2025-01-14T14:46:36-05:00",
          "tree_id": "0859862830ce38d974ee5888f09cc47e55b13093",
          "url": "https://github.com/paulmthompson/Whisker-Analysis/commit/264d118592213e97aa90c3e26571117dd4b1c00f"
        },
        "date": 1736884078578,
        "tool": "catch2",
        "benches": [
          {
            "name": "IoU Calculation",
            "value": 7.90806,
            "range": "± 489.939",
            "unit": "us",
            "extra": "100 samples\n4 iterations"
          },
          {
            "name": "IoU Relative Calculation",
            "value": 7.24077,
            "range": "± 382.635",
            "unit": "us",
            "extra": "100 samples\n5 iterations"
          },
          {
            "name": "Mask Intersection",
            "value": 29.8612,
            "range": "± 2.43715",
            "unit": "ns",
            "extra": "100 samples\n1044 iterations"
          },
          {
            "name": "length Calculation",
            "value": 277.806,
            "range": "± 0.494422",
            "unit": "ns",
            "extra": "100 samples\n107 iterations"
          },
          {
            "name": "Nearest Preceding Calculation",
            "value": 532.821,
            "range": "± 14.4062",
            "unit": "ns",
            "extra": "100 samples\n54 iterations"
          },
          {
            "name": "Nearest Interpolated Calculation",
            "value": 844.296,
            "range": "± 23.256",
            "unit": "ns",
            "extra": "100 samples\n36 iterations"
          }
        ]
      }
    ]
  }
}