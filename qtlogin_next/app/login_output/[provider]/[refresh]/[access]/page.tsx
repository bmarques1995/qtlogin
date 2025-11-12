'use client';
import { useParams } from 'next/navigation';
import { useEffect } from 'react';
import { validOriginsReverse } from '@/shared/validOrigins';

export default function Home() {
    const params = useParams<{ refresh: string; access: string, provider: string }>();

    console.log(validOriginsReverse.get(params.provider)!);
    console.log(params.refresh);
    console.log(params.access);

    useEffect(() => {
        const sendResponseToQt = () => {
            const payload = {
                refresh: params.refresh,
                access: params.access
            };

            // Encode JSON for URL (Base64 or percent)
            const json = encodeURIComponent(JSON.stringify(payload));
            const url = `${validOriginsReverse.get(params.provider)!}://open?data=${json}`;

            // Open the desktop app
            window.location.href = url;
        }

        sendResponseToQt();
    },[params]);

    return (
        <>
        </>
    );
}