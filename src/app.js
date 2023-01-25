const express = require('express')
const app = express()
const cors = require('cors')
app.set(express.json())
app.set(express.urlencoded({extended:true}))

const mysql = require('mysql2');


app.use(cors())
const conn = mysql.createPool({
    // connectionLimit : 10,
    host     : '127.0.0.1',
    user     : 'root',
    password : '',
    database : 'iot',
});    

conn.getConnection((err)=>{
     if (err) {
        console.log('db error');
        throw err
    }else{
        console.log('db connected');
    }
})

// update status
  app.get('/UpdateStatus/:status',async (req,res)=>{
    console.log(req.params.status);
    conn.query(`UPDATE iot SET status = ? WHERE id = 1 `,[req.params.status],(err,result)=>{
        if(err) throw err ;
    })
    res.send('success')
})

//fetch status
app.get('/getLedStatus', (req,res)=>{
    console.log('ok');
    conn.query('SELECT status FROM iot WHERE id = 1',(err,result)=>{
        if(err) throw err;
        
        if(result[0].status === 1){
            res.send('1')
        }else{
            res.send('0')
        }
    })
})


app.listen(3030,()=>{
    console.log('server running . .');
})